#include "utils/Compiler.hpp"
#include "utils/ProcessRunner.hpp"

#include <cassert>
#include <iostream>

int main() {
    Compiler compiler;

    auto compile_result = compiler.compile(
        "timeout_test",
        R"(int main() {
            while (true) {}
        })"
    );

    assert(compile_result.success);

    ProcessRunner runner;

    auto result = runner.run(
        compile_result.executable_path,
        "",
        100
    );

    assert(result.timeout);

    std::cout << "Timeout detected in "
              << result.execution_time_ms
              << " ms\n";

    std::cout << "Timeout test passed\n";
}