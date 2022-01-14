#pragma once

#include <chrono>

namespace TempEngineNamespace::Utils
{
    class Timer
    {
    public:
        Timer() :
            mStart(std::chrono::steady_clock::now()),
            mEnd({})
        {
        }

        void Start()
        {
            mEnd = std::chrono::steady_clock::time_point{};
            mStart = std::chrono::steady_clock::now();
        }

        void Stop()
        {
            mEnd = std::chrono::steady_clock::now();
        }

        std::chrono::microseconds GetDuration() const
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(mEnd - mStart);
        }

        float GetDurationSeconds() const
        {
            return (float)(GetDuration().count()) * 0.000001f;
        }

    private:
        std::chrono::steady_clock::time_point mStart;
        std::chrono::steady_clock::time_point mEnd;

    };
}
