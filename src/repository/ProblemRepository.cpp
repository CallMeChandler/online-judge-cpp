#include "repository/ProblemRepository.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

ProblemRepository::ProblemRepository(const std::string& file_path)
    : file_path(file_path) {
}

std::vector<Problem> ProblemRepository::getAll() const {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open problems file: " + file_path);
    }

    json data;
    file >> data;

    std::vector<Problem> problems;

    for (const auto& item : data) {
        Problem problem;

        problem.id = item.at("id");
        problem.title = item.at("title");
        problem.description = item.at("description");
        problem.difficulty = item.at("difficulty");

        problem.tags = item.at("tags").get<std::vector<std::string>>();
        problem.constraints =
            item.at("constraints").get<std::vector<std::string>>();

        for (const auto& test : item.at("sample_test_cases")) {
            SampleTestCase sample;

            sample.input = test.at("input");
            sample.expected_output = test.at("expected_output");

            problem.sample_test_cases.push_back(sample);
        }

        problems.push_back(problem);
    }

    return problems;
}

std::optional<Problem> ProblemRepository::getById(int id) const {
    const auto problems = getAll();

    for (const auto& problem : problems) {
        if (problem.id == id) {
            return problem;
        }
    }

    return std::nullopt;
}