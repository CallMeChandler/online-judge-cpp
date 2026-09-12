#pragma once

#include "models/Execution.hpp"

#include <optional>
#include <string>
#include <vector>

class ExecutionRepository {
public:
    explicit ExecutionRepository(const std::string& file_path);

    Execution create(Execution execution);

    void update(const Execution& execution);

    std::optional<Execution> getById(
        const std::string& id
    ) const;

    std::vector<Execution> getBySubmissionId(
        const std::string& submission_id
    ) const;

    std::vector<Execution> getAll() const;

    void updateExecution(const Execution& execution);

private:
    std::string file_path;

    static std::string generateId();
};