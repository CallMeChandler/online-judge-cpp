#pragma once

#include <string>

struct CompileResult {
    bool success;

    std::string executable_path;

    std::string compiler_output;
};

class Compiler {
public:
    CompileResult compile(
        const std::string& submission_id,
        const std::string& source_code
    ) const;
};