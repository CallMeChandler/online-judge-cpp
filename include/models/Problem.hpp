#pragma once

#include "models/TestCase.hpp"

#include <string>
#include <vector>

// Samples are shown to the user, hidden tests never are.
// Both are judged the same way, so they share one type.
using SampleTestCase = TestCase;

struct Problem {
    int id;
    std::string title;
    std::string description;
    std::string difficulty;

    std::vector<std::string> tags;
    std::vector<std::string> constraints;

    std::vector<TestCase> sample_test_cases;

    // Repository-only. Never serialized to an API response.
    std::vector<TestCase> hidden_test_cases;
};
