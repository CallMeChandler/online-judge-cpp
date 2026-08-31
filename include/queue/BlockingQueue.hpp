#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T>
class BlockingQueue {
public:
    BlockingQueue() = default;

    void push(T item);

    std::optional<T> pop();

    void shutdown();

    size_t size() const;

private:
    std::queue<T> queue;

    mutable std::mutex mutex;
    std::condition_variable condition;

    bool stopped = false;
};

template <typename T>
void BlockingQueue<T>::push(T item) {
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (stopped){
            return;
        }

        queue.push(std::move(item));
    }
    condition.notify_one();
}

template <typename T>
std::optional<T> BlockingQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(mutex);

    condition.wait(lock, [this]{
        return stopped || !queue.empty();
    });

    if (queue.empty()){
        return std::nullopt;
    }

    T item = std::move(queue.front());
    queue.pop();

    return item;
}

template <typename T>
void BlockingQueue<T>::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mutex);

        stopped=true;
    }

    condition.notify_all();
}

template <typename T>
size_t BlockingQueue<T>::size() const {
    std::lock_guard<std::mutex> lock(mutex);

    return queue.size();
}
