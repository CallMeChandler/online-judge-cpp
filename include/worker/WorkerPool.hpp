#pragma once

#include "models/JudgeJob.hpp"
#include "queue/BlockingQueue.hpp"

#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

class WorkerPool {
public:
    // This defines JobHandler as a function type
    using JobHandler = std::function<void(const JudgeJob&)>;

    WorkerPool(
        BlockingQueue<JudgeJob>& queue,
        std::size_t worker_count,
        JobHandler handler
    );

    ~WorkerPool();

    void start();
    void shutdown();

private:
    void workerLoop();

    BlockingQueue<JudgeJob>& queue;
    std::size_t worker_count;
    JobHandler handler;

    std::vector<std::thread> workers;
    bool started = false;
};