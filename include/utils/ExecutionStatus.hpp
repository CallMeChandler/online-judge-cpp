#pragma once

#include "models/Execution.hpp"

#include <string>

std::string executionStatusToString(ExecutionStatus status);

ExecutionStatus executionStatusFromString(
    const std::string& status
);
