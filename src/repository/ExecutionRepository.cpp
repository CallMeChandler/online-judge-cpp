#include "repository/ExecutionRepository.hpp"

#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

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

std::string verdictToString(Verdict verdict) {
    switch (verdict) {
        case Verdict::NONE:
            return "NONE";

        case Verdict::AC:
            return "AC";

        case Verdict::WA:
            return "WA";

        case Verdict::RE:
            return "RE";

        case Verdict::CE:
            return "CE";

        case Verdict::TLE:
            return "TLE";
    }

    throw std::invalid_argument("Invalid verdict");
}

Verdict verdictFromString(const std::string& verdict) {
    if (verdict == "NONE") {
        return Verdict::NONE;
    }

    if (verdict == "AC") {
        return Verdict::AC;
    }

    if (verdict == "WA") {
        return Verdict::WA;
    }

    if (verdict == "RE") {
        return Verdict::RE;
    }

    if (verdict == "CE") {
        return Verdict::CE;
    }

    if (verdict == "TLE") {
        return Verdict::TLE;
    }

    throw std::invalid_argument(
        "Invalid verdict: " + verdict
    );
}

json executionToJson(const Execution& execution) {
    json item;

    item["id"] = execution.id;
    item["submission_id"] = execution.submission_id;

    item["status"] =
        executionStatusToString(execution.status);

    item["verdict"] =
        verdictToString(execution.verdict);

    item["execution_time_ms"] =
        execution.execution_time_ms;

    item["memory_used_kb"] =
        execution.memory_used_kb;

    auto started_timestamp =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            execution.started_at.time_since_epoch()
        ).count();

    auto finished_timestamp =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            execution.finished_at.time_since_epoch()
        ).count();

    item["started_at"] = started_timestamp;
    item["finished_at"] = finished_timestamp;

    return item;
}

Execution jsonToExecution(const json& item) {
    Execution execution;

    execution.id = item.at("id");
    execution.submission_id = item.at("submission_id");

    execution.status =
        executionStatusFromString(item.at("status"));

    execution.verdict =
        verdictFromString(item.at("verdict"));

    execution.execution_time_ms =
        item.at("execution_time_ms");

    execution.memory_used_kb =
        item.at("memory_used_kb");

    auto started_timestamp =
        item.at("started_at").get<int64_t>();

    auto finished_timestamp =
        item.at("finished_at").get<int64_t>();

    execution.started_at =
        std::chrono::system_clock::time_point(
            std::chrono::milliseconds(started_timestamp)
        );

    execution.finished_at =
        std::chrono::system_clock::time_point(
            std::chrono::milliseconds(finished_timestamp)
        );

    return execution;
}

json loadFile(const std::string& file_path) {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Failed to open executions file: " + file_path
        );
    }

    json data;
    file >> data;

    return data;
}

void saveFile(
    const std::string& file_path,
    const json& data
) {
    std::ofstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Failed to open executions file: " + file_path
        );
    }

    file << data.dump(4);
}

} // namespace

ExecutionRepository::ExecutionRepository(
    const std::string& file_path
)
    : file_path(file_path) {
}

std::string ExecutionRepository::generateId() {
    static std::random_device random_device;
    static std::mt19937 generator(random_device());

    std::uniform_int_distribution<int> distribution(0, 15);

    std::stringstream stream;

    for (int i = 0; i < 16; ++i) {
        stream << std::hex << distribution(generator);
    }

    return stream.str();
}

Execution ExecutionRepository::create(
    Execution execution
) {
    execution.id = generateId();

    json data = loadFile(file_path);

    data.push_back(executionToJson(execution));

    saveFile(file_path, data);

    return execution;
}

void ExecutionRepository::update(
    const Execution& execution
) {
    json data = loadFile(file_path);

    for (auto& item : data) {
        if (item.at("id") == execution.id) {
            item = executionToJson(execution);
            saveFile(file_path, data);
            return;
        }
    }

    throw std::runtime_error(
        "Execution not found: " + execution.id
    );
}

std::optional<Execution>
ExecutionRepository::getById(
    const std::string& id
) const {
    json data = loadFile(file_path);

    for (const auto& item : data) {
        if (item.at("id") == id) {
            return jsonToExecution(item);
        }
    }

    return std::nullopt;
}

std::vector<Execution>
ExecutionRepository::getBySubmissionId(
    const std::string& submission_id
) const {
    json data = loadFile(file_path);

    std::vector<Execution> executions;

    for (const auto& item : data) {
        if (item.at("submission_id") == submission_id) {
            executions.push_back(
                jsonToExecution(item)
            );
        }
    }

    return executions;
}

std::vector<Execution>
ExecutionRepository::getAll() const {
    json data = loadFile(file_path);

    std::vector<Execution> executions;

    for (const auto& item : data) {
        executions.push_back(
            jsonToExecution(item)
        );
    }

    return executions;
}