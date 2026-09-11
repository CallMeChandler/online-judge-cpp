#pragma once

#include <string>

struct ProcessResult {
    int exit_code;

    std::string stdout_output;

    long long execution_time_ms;

    bool runtime_error;
};

class ProcessRunner {
public:
    ProcessResult run(
        const std::string& executable_path,
        const std::string& input
    ) const;
};