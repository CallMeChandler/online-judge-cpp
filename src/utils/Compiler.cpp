#include "utils/Compiler.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace fs = std::filesystem;

CompileResult Compiler::compile(
    const std::string& submission_id,
    const std::string& source_code
) const {
    fs::create_directories("../sandbox/src");
    fs::create_directories("../sandbox/bin");
    fs::create_directories("../sandbox/tmp");

    std::string source_path = "../sandbox/src/" + submission_id + ".cpp";

    std::string binary_path = "../sandbox/bin/" + submission_id;

    std::string log_path = "../sandbox/tmp/" + submission_id + ".log";

    {
        std::ofstream source_file(source_path);
        source_file << source_code;
    }

    std::string command = 
        "g++ -std=c++20 " +
        source_path +
        " -o " + 
        binary_path +
        " 2> " +
        log_path;

    int exit_code = std::system(command .c_str());

    std::ifstream log_file(log_path);
    
    std::stringstream buffer;
    buffer << log_file.rdbuf();

    CompileResult result;

    result.success = (exit_code == 0);
    result.executable_path = binary_path;
    result.compiler_output = buffer.str();

    return result;
}