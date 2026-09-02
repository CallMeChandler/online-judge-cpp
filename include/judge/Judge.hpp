#pragma once

#include "models/JudgeJob.hpp"
#include "repository/ExecutionRepository.hpp"
#include "repository/SubmissionRepository.hpp"

class Judge {
public:
    Judge(
        SubmissionRepository& submission_repository,
        ExecutionRepository& execution_repository
    );

    void judge(const JudgeJob& job);

private:
    SubmissionRepository& submission_repository;
    ExecutionRepository& execution_repository;
};