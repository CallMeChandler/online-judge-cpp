
#include "judge/Judge.hpp"

#include "repository/ExecutionRepository.hpp"
#include "repository/ProblemRepository.hpp"
#include "repository/SubmissionRepository.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

namespace {

// Problem 3 in data/problems.json ("A + B") has two samples:
//   "2 7"   -> "9"
//   "-4 10" -> "6"
constexpr int PROBLEM_ID = 3;

Execution runJudge(
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

    return execution;
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

    auto judge = [&](const std::string& source_code) {
        return runJudge(
            submission_repository,
            execution_repository,
            problem_repository,
            source_code
        );
    };

    // Regression: a correct program whose stdout ends with a
    // newline must be AC, not WA.
    auto correct = judge(
        "#include <iostream>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    std::cout << a + b << std::endl;\n"
        "}\n"
    );

    assert(correct.verdict == Verdict::AC);
    std::cout << "AC: trailing newline normalized\n";

    // Normalization must only trim whitespace, never make
    // a wrong answer pass.
    auto wrong = judge(
        "#include <iostream>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    std::cout << a - b << std::endl;\n"
        "}\n"
    );

    assert(wrong.verdict == Verdict::WA);
    std::cout << "WA: wrong answer still rejected\n";

    // Every sample runs: this passes sample 1 ("9") and fails
    // sample 2 ("6"). Judging only sample 1 would report AC.
    auto second_sample = judge(
        "#include <iostream>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    std::cout << 9 << std::endl;\n"
        "}\n"
    );

    assert(second_sample.verdict == Verdict::WA);
    std::cout << "WA: failure on the second sample is caught\n";

    // Fail fast: sample 1 is wrong, sample 2 would loop forever.
    // Continuing past the first failure would report TLE.
    auto fail_fast = judge(
        "#include <iostream>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    if (a == 2) {\n"
        "        std::cout << 0 << std::endl;\n"
        "        return 0;\n"
        "    }\n"
        "    while (true) {}\n"
        "}\n"
    );

    assert(fail_fast.verdict == Verdict::WA);
    assert(fail_fast.execution_time_ms < 1000);
    std::cout << "WA: stopped before running the remaining samples ("
              << fail_fast.execution_time_ms << " ms)\n";

    // Execution time is summed across samples: ~200 ms of sleep
    // per sample over two samples cannot total under 400 ms.
    auto summed = judge(
        "#include <chrono>\n"
        "#include <iostream>\n"
        "#include <thread>\n"
        "int main() {\n"
        "    int a, b;\n"
        "    std::cin >> a >> b;\n"
        "    std::this_thread::sleep_for(\n"
        "        std::chrono::milliseconds(200)\n"
        "    );\n"
        "    std::cout << a + b << std::endl;\n"
        "}\n"
    );

    assert(summed.verdict == Verdict::AC);
    assert(summed.execution_time_ms >= 400);
    std::cout << "AC: time summed across samples ("
              << summed.execution_time_ms << " ms)\n";

    std::cout << "Judge test passed\n";

    return 0;
}
