#include "models/JudgeJob.hpp"
#include "queue/BlockingQueue.hpp"
#include "worker/WorkerPool.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

int main() {

    BlockingQueue<JudgeJob> queue;

    WorkerPool pool(queue, 3);

    pool.start();

    JudgeJob job;

    job.submission_id = "submission-123";

    queue.push(job);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)
    );

    pool.shutdown();

    std::cout << "WorkerPool test passed\n";

    return 0;
}