#pragma once

#include <chrono>
#include <string>

enum class SubmissionStatus {
    QUEUED,
    COMPILING,
    RUNNING,
    AC,
    WA,
    RE,
    CE,
    TLE
};

struct Submission {
    std::string id;

    int problem_id;

    std::string source_code;
    std::string language;

    SubmissionStatus status;

    std::chrono::system_clock::time_point created_at;
};