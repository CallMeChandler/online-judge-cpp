#include "judge/Judge.hpp"

#include <chrono>
#include <stdexcept>
#include <iostream>

Judge::Judge(
    SubmissionRepository& submission_repository,
    ExecutionRepository& execution_repository
)
    : submission_repository(submission_repository),
      execution_repository(execution_repository) {
}

void Judge::judge(const JudgeJob& job) {
    auto submission = submission_repository.getById(job.submission_id);

    std::cout
        << "[Judge] Evaluating submission "
        << submission->id
        << std::endl;

    if (!submission) {
        throw std::runtime_error(
            "Submission not found: " + job.submission_id
        );
    }

    Execution execution;
    
    execution.submission_id = submission->id;

    execution.status =
        ExecutionStatus::COMPILING;

    execution.verdict =
        Verdict::NONE;

    execution.execution_time_ms = 0;
    execution.memory_used_kb = 0;

    execution.started_at =
        std::chrono::system_clock::now();

    execution.finished_at =
        execution.started_at;

    execution =
        execution_repository.create(
            std::move(execution)
        );
}