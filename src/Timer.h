#ifndef TIMER_H
#define TIMER_H

#include <vector>
#include <chrono>
#include <functional>
#include <thread>
#include <algorithm>
#include <mutex>
#include <condition_variable>

#pragma once

class Timer
{
public:
    Timer();

    void start();
    void stop();

    void registerFunction(std::function<void()>);
    void unregisterFunction(std::function<void()>);

    void setInterval(double);

private:
    void countdown();
    void callFunctions();

    std::chrono::duration<double> m_interval;
    std::vector<std::function<void()>> m_functionRegistry;
    std::chrono::high_resolution_clock::time_point m_last_time;
    std::thread m_timer_thread; 
    std::mutex m_mutex;
    std::condition_variable m_convar;
    bool m_halted;
};

#endif