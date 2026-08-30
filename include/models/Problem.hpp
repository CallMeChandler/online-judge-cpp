#pragma once

#include <string>
#include <vector>

struct SampleTestCase{
    std::string input;
    std::string expected_output;
};

struct Problem {
    int id;
    std::string title;
    std::string description;
    std::string difficulty;

    std::vector<std::string> tags;
    std::vector<std::string> constraints;

    std::vector<SampleTestCase> sample_test_cases;
};