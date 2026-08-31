#pragma once

#include "models/JudgeJob.hpp"
#include "queue/BlockingQueue.hpp"

#include <cstddef>
#include <thread>
#include <vector>

class WorkerPool {
public:
    WorkerPool(
        BlockingQueue<JudgeJob>& queue,
        std::size_t worker_count
    );

    ~WorkerPool();
    
    void start();

    void shutdown();

private:
    void workerLoop();

    BlockingQueue<JudgeJob>& queue;

    std::size_t worker_count;

    std::vector<std::thread> workers;

    bool started = false;
};