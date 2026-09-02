#include "service/SubmissionService.hpp"

#include <chrono>
#include <stdexcept>
#include <utility>

SubmissionService::SubmissionService(
    SubmissionRepository repository,
    ProblemService& problem_service,
    BlockingQueue<JudgeJob>& judge_queue
)
    : repository(std::move(repository)),
      problem_service(problem_service),
      judge_queue(judge_queue) {
}

Submission SubmissionService::createSubmission(
    int problem_id,
    const std::string& source_code,
    const std::string& language
) {
    if (!problem_service.getProblemById(problem_id)) {
        throw std::invalid_argument("Problem not found");
    }

    if (source_code.empty()) {
        throw std::invalid_argument("Source code cannot be empty");
    }

    if (language != "cpp") {
        throw std::invalid_argument("Only cpp is currently supported");
    }

    Submission submission;

    submission.problem_id = problem_id;
    submission.source_code = source_code;
    submission.language = language;
    submission.status = SubmissionStatus::QUEUED;
    submission.created_at = std::chrono::system_clock::now();

    Submission saved_submission =
    repository.create(std::move(submission));

    JudgeJob job;
    job.submission_id = saved_submission.id;

    judge_queue.push(std::move(job));

    return saved_submission;
}

std::optional<Submission>
SubmissionService::getSubmissionById(
    const std::string& id
) const {
    return repository.getById(id);
}