#include "repository/SubmissionRepository.hpp"
#include "utils/SubmissionStatus.hpp"

#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

json submissionToJson(const Submission& submission) {
    json item;

    item["id"] = submission.id;
    item["problem_id"] = submission.problem_id;
    item["source_code"] = submission.source_code;
    item["language"] = submission.language;
    item["status"] = submissionStatusToString(submission.status);

    auto timestamp =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            submission.created_at.time_since_epoch()
        ).count();

    item["created_at"] = timestamp;

    return item;
}

Submission jsonToSubmission(const json& item) {
    Submission submission;

    submission.id = item.at("id");
    submission.problem_id = item.at("problem_id");
    submission.source_code = item.at("source_code");
    submission.language = item.at("language");
    submission.status = submissionStatusFromString(item.at("status"));

    auto timestamp = item.at("created_at").get<int64_t>();

    submission.created_at =
        std::chrono::system_clock::time_point(
            std::chrono::milliseconds(timestamp)
        );

    return submission;
}

json loadFile(const std::string& file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Failed to open submissions file: " + file_path
        );
    }

    json data;
    file >> data;

    return data;
}

void saveFile(const std::string& file_path, const json& data) {
    std::ofstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Failed to open submissions file: " + file_path
        );
    }

    file << data.dump(4);
}

} // namespace

SubmissionRepository::SubmissionRepository(const std::string& file_path)
    : file_path(file_path) {
}

std::string SubmissionRepository::generateId() {
    static std::random_device random_device;
    static std::mt19937 generator(random_device());

    std::uniform_int_distribution<int> distribution(0, 15);

    std::stringstream stream;

    for (int i = 0; i < 16; i++) {
        stream << std::hex << distribution(generator);
    }

    return stream.str();
}

Submission SubmissionRepository::create(Submission submission) {
    submission.id = generateId();

    json data = loadFile(file_path);

    data.push_back(submissionToJson(submission));

    saveFile(file_path, data);

    return submission;
}

std::optional<Submission>
SubmissionRepository::getById(const std::string& id) const {

    json data = loadFile(file_path);

    for (const auto& item : data) {
        if (item.at("id") == id) {
            return jsonToSubmission(item);
        }
    }

    return std::nullopt;
}

std::vector<Submission>
SubmissionRepository::getAll() const {

    json data = loadFile(file_path);

    std::vector<Submission> submissions;

    for (const auto& item : data) {
        submissions.push_back(jsonToSubmission(item));
    }

    return submissions;
}