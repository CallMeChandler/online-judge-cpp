#pragma once

#include "models/Submission.hpp"
#include "repository/SubmissionRepository.hpp"
#include "service/ProblemService.hpp"

#include <optional>
#include <string>

class SubmissionService {
public:
    SubmissionService(
        SubmissionRepository repository,
        ProblemService& problem_service
    );

    Submission createSubmission(
        int problem_id,
        const std::string& source_code,
        const std::string& language
    );

    std::optional<Submission> getSubmissionById(
        const std::string& id
    ) const;

private:
    SubmissionRepository repository;
    ProblemService& problem_service;
};