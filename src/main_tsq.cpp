// src/main_tsq.cpp
#include "thread_safe_queue.hpp"
#include <thread>
#include <vector>
#include <iostream>
#include <functional>
#include <mutex>

static std::mutex cout_mtx;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mtx);
    std::cout << msg << '\n';
}

void worker(ThreadSafeQueue& q, int id) {
    q.push(id);
    safe_print("Thread " + std::to_string(id) + " pushed");

    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    safe_print("Thread " + std::to_string(id) + " sees front: " + std::to_string(q.front()));
    q.pop();
}

int main() {
    ThreadSafeQueue q;
    std::vector<std::thread> threads;

    for (int i = 1; i <= 4; ++i) {
        threads.emplace_back(worker, std::ref(q), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final size: " << q.size() << '\n';
}