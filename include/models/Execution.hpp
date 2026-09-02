#pragma once

#include <string>
#include <chrono>

enum class ExecutionStatus {
    QUEUED,
    COMPILING,
    RUNNING,
    FINISHED
};

enum class Verdict {
    NONE,
    AC,
    WA,
    RE,
    CE,
    TLE
};

struct Execution {
    std::string id;

    std::string submission_id;

    ExecutionStatus status;
    Verdict verdict;

    long long execution_time_ms;
    long long memory_used_kb;

    std::chrono::system_clock::time_point started_at;
    std::chrono::system_clock::time_point finished_at;
};