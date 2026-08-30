#pragma once

#include "models/Submission.hpp"

#include <optional>
#include <string>
#include <vector>

class SubmissionRepository {
public:
    explicit SubmissionRepository(const std::string& file_path);

    Submission create(Submission submission);

    std::optional<Submission> getById(const std::string& id) const;

    std::vector<Submission> getAll() const;

private:
    std::string file_path;

    static std::string generateId();
};