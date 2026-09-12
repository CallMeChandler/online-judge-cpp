#pragma once

#include "models/Execution.hpp"

#include <string>

std::string verdictToString(Verdict verdict);

Verdict verdictFromString(const std::string& verdict);
