#pragma once

#include "models/Problem.hpp"

#include <optional>
#include <string>
#include <vector>

class ProblemRepository{
public:
    explicit ProblemRepository(const std::string& file_path);

    std::vector<Problem> getAll() const;

    std::optional<Problem> getById(int id) const;

private:
    std::string file_path;
};