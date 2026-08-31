#include "worker/WorkerPool.hpp"

#include <stdexcept>

WorkerPool::WorkerPool(
    BlockingQueue<JudgeJob>& queue,
    std::size_t worker_count
)
    : queue(queue),
      worker_count(worker_count) {

    if (worker_count == 0) {
        throw std::invalid_argument(
            "Worker count must be greater than zero"
        );
    }
}

WorkerPool::~WorkerPool() {
    shutdown();
}

void WorkerPool::start() {

    if (started) {
        return;
    }

    started = true;

    workers.reserve(worker_count);

    for (std::size_t i = 0; i < worker_count; ++i) {

        workers.emplace_back(
            [this] {
                workerLoop();
            }
        );
    }
}

void WorkerPool::workerLoop() {

    while (true) {

        auto job = queue.pop();

        if (!job) {
            break;
        }

        // Temporary behavior.
        // Actual judging will be implemented later.
    }
}

void WorkerPool::shutdown() {

    if (!started) {
        return;
    }

    queue.shutdown();

    for (auto& worker : workers) {

        if (worker.joinable()) {
            worker.join();
        }
    }

    workers.clear();

    started = false;
}