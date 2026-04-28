#pragma once

#include <chrono>

class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;

    void start() {
        start_time_ = Clock::now();
    }

    long long stopNanoseconds() const {
        auto end_time = Clock::now();

        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            end_time - start_time_
        ).count();
    }

private:
    Clock::time_point start_time_;
};