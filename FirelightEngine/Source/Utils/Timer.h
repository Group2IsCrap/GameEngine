#pragma once

#include <chrono>

namespace Firelight::Utils
{
    class Timer
    {
    public:
        Timer() :
            m_start(std::chrono::steady_clock::now()),
            m_end({})
        {
        }

        void Start()
        {
            m_end = std::chrono::steady_clock::time_point{};
            m_start = std::chrono::steady_clock::now();
        }

        void Stop()
        {
            m_end = std::chrono::steady_clock::now();
        }

        std::chrono::microseconds GetDuration() const
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start);
        }

        double GetDurationSeconds() const
        {
            return (double)(GetDuration().count()) * 0.000001;
        }

    private:
        std::chrono::steady_clock::time_point m_start;
        std::chrono::steady_clock::time_point m_end;

    };
}
