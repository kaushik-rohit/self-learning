#include<timer.hpp>

Timer::Timer(){
    reset();
}

void Timer::reset(){
    t0 = std::chrono::steady_clock::now();
}

double Timer::ms() const {
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}