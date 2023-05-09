#pragma once
#include <array>
#include <chrono>

namespace CommonUtilities
{
    class TimerClock
    {
    public:
        TimerClock();
        TimerClock(const TimerClock& aTimer) = delete;
        TimerClock& operator=(const TimerClock& aTimer) = delete;

        void Update();

        int GetFPS();
        float GetDeltaTime() const;
        double GetTotalTime() const;
        int GetElapsedFrames() const;

    private:

        float CalculateFPS() const;
        int lastFrameIndex = 0;
        std::array<float, 50> myFrameValues{};
        int myFramesPerSecond = 0;
        unsigned int myElapsedFrames = 0;
        float myDeltaTime;
        double myTotalTimeElapsed = 0;
        std::chrono::steady_clock::time_point myStartTime;
    };
}