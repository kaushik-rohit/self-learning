#include "thread_safe_queue.hpp"
#include <stdexcept>

int ThreadSafeQueue::front(){
    std::lock_guard<std::mutex> lock(mtx_);
    if (data.empty()) {
        throw std::runtime_error("The queue is empty.");
    }
    return data[0];
}

void ThreadSafeQueue::pop() {
    std::lock_guard<std::mutex> lock(mtx_);

    if(data.size() == 0) {
        throw std::runtime_error("The queue is empty.");
    }
    data.erase(data.begin());
}

void ThreadSafeQueue::push(int val) {
    std::lock_guard<std::mutex> lock(mtx_);
    data.push_back(val);
}

int ThreadSafeQueue::size() {
    std::lock_guard<std::mutex> lock(mtx_);
    return data.size();
}

bool ThreadSafeQueue::empty() {
    std::lock_guard<std::mutex> lock(mtx_);
    return data.empty();
}