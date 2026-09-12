#include "utils/ExecutionStatus.hpp"

#include <stdexcept>

std::string executionStatusToString(ExecutionStatus status) {
    switch (status) {
        case ExecutionStatus::QUEUED:
            return "QUEUED";

        case ExecutionStatus::COMPILING:
            return "COMPILING";

        case ExecutionStatus::RUNNING:
            return "RUNNING";

        case ExecutionStatus::FINISHED:
            return "FINISHED";
    }

    throw std::invalid_argument("Invalid execution status");
}

ExecutionStatus executionStatusFromString(
    const std::string& status
) {
    if (status == "QUEUED") {
        return ExecutionStatus::QUEUED;
    }

    if (status == "COMPILING") {
        return ExecutionStatus::COMPILING;
    }

    if (status == "RUNNING") {
        return ExecutionStatus::RUNNING;
    }

    if (status == "FINISHED") {
        return ExecutionStatus::FINISHED;
    }

    throw std::invalid_argument(
        "Invalid execution status: " + status
    );
}
