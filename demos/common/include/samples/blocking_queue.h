#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>

template<typename T>
class blocking_queue {
public:
    void push(T const &value, size_t queue_limit = 0) {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue_limit > 0) {
            while (queue.size() >= queue_limit) {
                pop_condition.wait(lock);
            }
        }
        queue.push_front(value);
        push_condition.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        push_condition.wait(lock, [=] { return !queue.empty(); });
        T value(std::move(queue.back()));
        queue.pop_back();
        pop_condition.notify_one();
        return value;
    }

    void clear() {
        queue.clear();
        pop_condition.notify_one();
    }

    size_t size() {
        return queue.size();
    }

private:
    std::deque<T> queue;
    std::mutex mutex;
    std::condition_variable push_condition;
    std::condition_variable pop_condition;
};
