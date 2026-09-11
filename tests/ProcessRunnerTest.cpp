#include "utils/Compiler.hpp"
#include "utils/ProcessRunner.hpp"

#include <cassert>
#include <iostream>

int main() {

    Compiler compiler;

    auto compile_result =
        compiler.compile(
            "runner_test",
            R"(#include <iostream>

int main() {
    std::cout << 42;
})"
        );

    assert(compile_result.success);

    ProcessRunner runner;

    auto result =
        runner.run(
            compile_result.executable_path
        );

    assert(result.exit_code == 0);
    assert(result.stdout_output == "42");

    std::cout
        << "Output: "
        << result.stdout_output
        << '\n';

    std::cout
        << "Execution time: "
        << result.execution_time_ms
        << " ms\n";

    std::cout
        << "ProcessRunner test passed\n";
}