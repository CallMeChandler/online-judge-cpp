
#include "judge/Judge.hpp"

#include "repository/ExecutionRepository.hpp"
#include "repository/ProblemRepository.hpp"
#include "repository/SubmissionRepository.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

namespace {

// Problem 3 in data/problems.json ("A + B").
// First sample: "2 7" -> "9".
constexpr int PROBLEM_ID = 3;

Verdict runJudge(
    SubmissionRepository& submission_repository,
    ExecutionRepository& execution_repository,
    ProblemRepository& problem_repository,
    const std::string& source_code
) {
    Submission submission =
        submission_repository.create(
            Submission{
                "",
                PROBLEM_ID,
                source_code,
                "cpp",
                SubmissionStatus::QUEUED,
                std::chrono::system_clock::now()
            }
        );

    Judge judge(
        submission_repository,
        execution_repository,
        problem_repository
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
    assert(execution.status == ExecutionStatus::FINISHED);

    return execution.verdict;
}

} // namespace

int main() {
    ProblemRepository problem_repository(
        "../data/problems.json"
    );

    SubmissionRepository submission_repository(
        "../data/submissions.json"
    );

    ExecutionRepository execution_repository(
        "../data/executions.json"
    );

    // Regression: a correct program whose stdout ends with a
    // newline must be AC, not WA.
    Verdict correct = runJudge(
        submission_repository,
        execution_repository,
        problem_repository,
        "#include <iostream>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    std::cout << a + b << std::endl;\n"
        "}\n"
    );

    assert(correct == Verdict::AC);
    std::cout << "Verdict: AC (trailing newline normalized)\n";

    // Normalization must only trim whitespace, never make
    // a wrong answer pass.
    Verdict wrong = runJudge(
        submission_repository,
        execution_repository,
        problem_repository,
        "#include <iostream>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    std::cout << a - b << std::endl;\n"
        "}\n"
    );

    assert(wrong == Verdict::WA);
    std::cout << "Verdict: WA (wrong answer still rejected)\n";

    std::cout << "Judge test passed\n";

    return 0;
}
