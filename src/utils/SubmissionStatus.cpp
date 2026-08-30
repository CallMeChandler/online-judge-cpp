#include "utils/SubmissionStatus.hpp"

#include <stdexcept>

std::string submissionStatusToString(
    SubmissionStatus status
) {
    switch (status) {
        case SubmissionStatus::QUEUED:
            return "QUEUED";

        case SubmissionStatus::COMPILING:
            return "COMPILING";

        case SubmissionStatus::RUNNING:
            return "RUNNING";

        case SubmissionStatus::AC:
            return "AC";

        case SubmissionStatus::WA:
            return "WA";

        case SubmissionStatus::RE:
            return "RE";

        case SubmissionStatus::CE:
            return "CE";

        case SubmissionStatus::TLE:
            return "TLE";
    }

    throw std::runtime_error("Unknown submission status");
}

SubmissionStatus submissionStatusFromString(
    const std::string& status
) {
    if (status == "QUEUED")
        return SubmissionStatus::QUEUED;

    if (status == "COMPILING")
        return SubmissionStatus::COMPILING;

    if (status == "RUNNING")
        return SubmissionStatus::RUNNING;

    if (status == "AC")
        return SubmissionStatus::AC;

    if (status == "WA")
        return SubmissionStatus::WA;

    if (status == "RE")
        return SubmissionStatus::RE;

    if (status == "CE")
        return SubmissionStatus::CE;

    if (status == "TLE")
        return SubmissionStatus::TLE;

    throw std::runtime_error(
        "Unknown submission status: " + status
    );
}