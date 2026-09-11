#include "utils/ProcessRunner.hpp"

#include <array>
#include <chrono>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

ProcessResult ProcessRunner::run(
    const std::string& executable_path,
    const std::string& input
) const {

    std::string input_path =
        "../sandbox/tmp/input.txt";

    std::ofstream input_file(input_path);
    input_file << input;
    input_file.close();

    int pipefd[2];
    pipe(pipefd);

    auto start = std::chrono::steady_clock::now();

    pid_t pid = fork();

    if (pid == 0) {

        int input_fd =
            open(input_path.c_str(), O_RDONLY);

        dup2(input_fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[0]);
        close(pipefd[1]);
        close(input_fd);

        execl(
            executable_path.c_str(),
            executable_path.c_str(),
            nullptr
        );

        _exit(1);
    }

    close(pipefd[1]);

    std::array<char, 256> buffer;

    std::string output;

    ssize_t bytes;

    while ((bytes = read(
        pipefd[0],
        buffer.data(),
        buffer.size()
    )) > 0) {
        output.append(buffer.data(), bytes);
    }

    close(pipefd[0]);

    int status;

    waitpid(pid, &status, 0);

    auto end = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(end - start).count();

    ProcessResult result;

    result.execution_time_ms = elapsed;
    result.stdout_output = output;

    if (WIFEXITED(status)) {
        result.exit_code = WEXITSTATUS(status);
        result.runtime_error = false;
    }
    else {
        result.exit_code = -1;
        result.runtime_error = true;
    }

    return result;
}