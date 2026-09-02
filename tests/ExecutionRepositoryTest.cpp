#include "models/Execution.hpp"
#include "repository/ExecutionRepository.hpp"

#include <cassert>
#include <chrono>
#include <iostream>

int main() {
    ExecutionRepository repository(
        "../data/executions.json"
    );

    Execution execution;

    execution.submission_id = "test-submission";
    execution.status = ExecutionStatus::QUEUED;
    execution.verdict = Verdict::NONE;

    execution.execution_time_ms = 0;
    execution.memory_used_kb = 0;

    execution.started_at =
        std::chrono::system_clock::now();

    execution.finished_at =
        execution.started_at;

    Execution created =
        repository.create(execution);

    std::cout
        << "Created execution: "
        << created.id
        << '\n';

    // Test getById()
    auto found =
        repository.getById(created.id);

    assert(found.has_value());
    assert(found->id == created.id);
    assert(found->submission_id == "test-submission");

    std::cout
        << "getById() passed\n";

    // Test getBySubmissionId()
    auto executions =
        repository.getBySubmissionId(
            "test-submission"
        );

    assert(!executions.empty());

    bool found_created_execution = false;

    for (const auto& item : executions) {
        if (item.id == created.id) {
            found_created_execution = true;
            break;
        }
    }

    assert(found_created_execution);

    std::cout
        << "getBySubmissionId() passed\n";

    std::cout
        << "ExecutionRepository test passed\n";

    return 0;
}