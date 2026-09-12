#include "judge/Judge.hpp"

#include <chrono>
#include <string>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace {

// Per test case, not per submission.
constexpr long long TIME_LIMIT_MS = 1000;

// Judges compare logical output, not raw bytes.
// Programs almost always end stdout with a newline while
// expected_output in problems.json does not, so trim the
// leading/trailing whitespace of both sides before comparing.
std::string normalizeOutput(const std::string& output) {
    static const std::string whitespace = " \t\n\r\f\v";

    auto first = output.find_first_not_of(whitespace);

    if (first == std::string::npos) {
        return "";
    }

    auto last = output.find_last_not_of(whitespace);

    return output.substr(first, last - first + 1);
}

} // namespace

Judge::Judge(
    SubmissionRepository& submission_repository,
    ExecutionRepository& execution_repository,
    ProblemRepository& problem_repository
)
    : submission_repository(submission_repository),
      execution_repository(execution_repository),
      problem_repository(problem_repository) {}

void Judge::judge(const JudgeJob& job) {
    auto submission = submission_repository.getById(job.submission_id);

    if (!submission) {
        throw std::runtime_error(
            "Submission not found: " + job.submission_id
        );
    }

    std::cout << "[Judge] Evaluating submission "
              << submission->id << std::endl;

    auto problem =
        problem_repository.getById(submission->problem_id);

    if (!problem) {
        throw std::runtime_error("Problem not found");
    }

    Execution execution;

    execution.submission_id = submission->id;
    execution.status = ExecutionStatus::COMPILING;
    execution.verdict = Verdict::NONE;
    execution.execution_time_ms = 0;
    execution.memory_used_kb = 0;
    execution.started_at = std::chrono::system_clock::now();
    execution.finished_at = execution.started_at;

    execution = execution_repository.create(std::move(execution));

    auto compile_result = compiler.compile(
        submission->id,
        submission->source_code
    );

    if (!compile_result.success) {
        execution.status = ExecutionStatus::FINISHED;
        execution.verdict = Verdict::CE;
        execution.finished_at = std::chrono::system_clock::now();

        execution_repository.updateExecution(execution);
        return;
    }

    execution.status = ExecutionStatus::RUNNING;
    execution_repository.updateExecution(execution);

    // Every sample runs sequentially. The first failing sample
    // decides the verdict and the remaining ones are skipped,
    // so execution_time_ms is the sum over the samples that
    // actually ran.
    long long total_time_ms = 0;
    Verdict verdict = Verdict::AC;

    for (const auto& test : problem->sample_test_cases) {
        auto result = runner.run(
            compile_result.executable_path,
            test.input,
            TIME_LIMIT_MS
        );

        total_time_ms += result.execution_time_ms;

        if (result.timeout) {
            verdict = Verdict::TLE;
            break;
        }

        if (result.runtime_error) {
            verdict = Verdict::RE;
            break;
        }

        if (normalizeOutput(result.stdout_output) !=
            normalizeOutput(test.expected_output)) {
            verdict = Verdict::WA;
            break;
        }
    }

    execution.status = ExecutionStatus::FINISHED;
    execution.finished_at = std::chrono::system_clock::now();
    execution.execution_time_ms = total_time_ms;
    execution.verdict = verdict;

    execution_repository.updateExecution(execution);
}