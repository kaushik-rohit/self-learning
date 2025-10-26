#pragma once
#include <chrono>
#include <string>

class Timer {
public:
    Timer();
    void reset();
    double ms() const;
private:
    std::chrono::time_point<std::chrono::steady_clock> t0;
};