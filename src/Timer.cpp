#include "Timer.h"

using namespace std::chrono;

Timer::Timer():m_interval(duration<double>(0)), 
               m_timer_thread(&Timer::countdown),
               m_halted(false)
{}


void Timer::start()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_halted = false;
    m_convar.notify_all();
}

void Timer::stop()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_halted = true;
}


void Timer::registerFunction(std::function<void()> function)
{
    std::lock_guard<std::mutex> lock(m_mutex); 
    std::vector<std::function<void()>>::iterator it = std::find(m_functionRegistry.begin(), m_functionRegistry.end(), function);

    if (it != m_functionRegistry.end()) m_functionRegistry.push_back(function);
}

void Timer::unregisterFunction(std::function<void()> function)
{
    std::lock_guard<std::mutex> lock(m_mutex); 
    std::vector<std::function<void()>>::iterator it = std::find(m_functionRegistry.begin(), m_functionRegistry.end(), function);
    
    if (it == m_functionRegistry.end()) m_functionRegistry.erase(it);
}

void Timer::setInterval(double interval)
{
    std::lock_guard<std::mutex> lock(m_mutex); 
    m_interval = std::chrono::duration<double>(interval);
}

void Timer::countdown()
{
    m_last_time = high_resolution_clock::now();

    while(true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_halted) m_convar.wait(lock);

        high_resolution_clock::time_point current_time = high_resolution_clock::now();
        duration<double> current_interval = duration_cast<duration<double>>(current_time - m_last_time);
        if (current_interval >= m_interval)
        {
            callFunctions();
            m_last_time = current_time;
        }
    }

}

void Timer::callFunctions()
{
    std::lock_guard<std::mutex> lock(m_mutex); 
    for (const auto& function: m_functionRegistry) function();      
}