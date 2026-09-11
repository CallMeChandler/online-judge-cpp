#include "utils/ProcessRunner.hpp"

#include <array>
#include <chrono>
#include <cstdio>
#include <sys/wait.h>

ProcessResult ProcessRunner::run(
    const std::string& executable_path
) const {
    auto start = std::chrono::steady_clock::now();

    FILE* pipe = popen(executable_path.c_str(), "r");

    if (!pipe){
        return {-1, "", 0};
    }

    std::array<char, 256> buffer;

    std::string output;

    while (fgets(buffer.data(), buffer.size(), pipe)){
        output += buffer.data();
    }

    int status = pclose(pipe);

    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    int exit_code = WEXITSTATUS(status);

    return {
        exit_code,
        output,
        elapsed
    };
}