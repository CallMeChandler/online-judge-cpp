#include "models/JudgeJob.hpp"
#include "queue/BlockingQueue.hpp"
#include "worker/WorkerPool.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    BlockingQueue<JudgeJob> queue;

    std::atomic<int> processed_jobs{0};

    WorkerPool pool(
        queue,
        3,
        [&processed_jobs](const JudgeJob&) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(200)
            );

            ++processed_jobs;
        }
    );

    pool.start();

    for (int i = 0; i < 6; ++i) {
        JudgeJob job;
        job.submission_id =
            "submission-" + std::to_string(i);

        queue.push(std::move(job));
    }

    pool.shutdown();

    std::cout << "Processed jobs: "
            << processed_jobs.load()
            << '\n';

    assert(processed_jobs == 6);

    std::cout
        << "Processed jobs: "
        << processed_jobs.load()
        << '\n';

    std::cout << "WorkerPool concurrency test passed\n";

    return 0;
}