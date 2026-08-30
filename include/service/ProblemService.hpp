#pragma once

#include "models/Problem.hpp"
#include "repository/ProblemRepository.hpp"

#include <optional>
#include <vector>

class ProblemService {
public:
    explicit ProblemService(ProblemRepository repository);

    std::vector<Problem> getAllProblems() const;

    std::optional<Problem> getProblemById(int id) const;

private:
    ProblemRepository repository;
};