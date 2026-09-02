#include "judge/Judge.hpp"

#include "repository/ProblemRepository.hpp"
#include "service/ProblemService.hpp"

#include <cassert>
#include <iostream>

int main() {

    SubmissionRepository submission_repository(
        "../data/submissions.json"
    );

    ExecutionRepository execution_repository(
        "../data/executions.json"
    );

    // Create a real submission first.
    Submission submission =
        submission_repository.create(
            Submission{
                "",
                1,
                "int main() { return 0; }",
                "cpp",
                SubmissionStatus::QUEUED,
                std::chrono::system_clock::now()
            }
        );

    Judge judge(
        submission_repository,
        execution_repository
    );

    JudgeJob job;
    job.submission_id = submission.id;

    judge.judge(job);

    auto executions =
        execution_repository.getBySubmissionId(
            submission.id
        );

    assert(!executions.empty());

    const Execution& execution = executions.back();

    assert(execution.submission_id == submission.id);
    assert(execution.status == ExecutionStatus::COMPILING);
    assert(execution.verdict == Verdict::NONE);

    std::cout
        << "Judge created execution: "
        << execution.id
        << '\n';

    std::cout
        << "Judge test passed\n";

    return 0;
}