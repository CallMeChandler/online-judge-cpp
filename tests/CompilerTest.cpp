#include "utils/Compiler.hpp"

#include <cassert>
#include <iostream>

int main() {

    Compiler compiler;

    auto result =
        compiler.compile(
            "compiler_test",
            R"(#include <iostream>

int main() {
    std::cout << 42;
})"
        );

    assert(result.success);

    std::cout
        << "Executable: "
        << result.executable_path
        << '\n';

    std::cout
        << "Compiler test passed\n";
}