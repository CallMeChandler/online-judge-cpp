#include "queue/BlockingQueue.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

int main() {

    // --------------------------------------------------
    // Test 1: Producer wakes waiting consumer
    // --------------------------------------------------

    {
        BlockingQueue<int> queue;

        bool received = false;

        std::thread worker([&] {
            auto item = queue.pop();

            if (item && *item == 42) {
                received = true;
            }
        });

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100)
        );

        queue.push(42);

        worker.join();

        assert(received);
    }


    // --------------------------------------------------
    // Test 2: Shutdown wakes waiting consumer
    // --------------------------------------------------

    {
        BlockingQueue<int> queue;

        bool worker_exited = false;

        std::thread worker([&] {

            auto item = queue.pop();

            // Shutdown should cause pop() to return nullopt.
            assert(!item);

            worker_exited = true;
        });

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100)
        );

        queue.shutdown();

        worker.join();

        assert(worker_exited);
    }


    // --------------------------------------------------
    // Test 3: Push after shutdown is ignored
    // --------------------------------------------------

    {
        BlockingQueue<int> queue;

        queue.shutdown();

        queue.push(123);

        auto item = queue.pop();

        assert(!item);
    }


    std::cout << "All BlockingQueue tests passed\n";

    return 0;
}