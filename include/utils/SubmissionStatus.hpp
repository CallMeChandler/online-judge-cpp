#pragma once

#include "models/Submission.hpp"

#include <string>

std::string submissionStatusToString(SubmissionStatus status);

SubmissionStatus submissionStatusFromString(
    const std::string& status
);