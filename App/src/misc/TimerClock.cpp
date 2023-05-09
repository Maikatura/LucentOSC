#include "TimerClock.h"

CommonUtilities::TimerClock::TimerClock(): myDeltaTime(0)
{
	myStartTime = std::chrono::high_resolution_clock::now();
}

void CommonUtilities::TimerClock::Update()
{
    std::chrono::steady_clock::time_point clockNow = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = clockNow - myStartTime;
    myStartTime = clockNow;
    myDeltaTime = static_cast<float>(time.count()) / 1000.0f;
    myTotalTimeElapsed += static_cast<double>(myDeltaTime);
    ++myElapsedFrames;

    myFrameValues[lastFrameIndex] = myDeltaTime;
    lastFrameIndex = (lastFrameIndex + 1) % myFrameValues.size();

    myFramesPerSecond = static_cast<int>(CalculateFPS());
}

float CommonUtilities::TimerClock::CalculateFPS() const
{
    float total = 0.0f;
    for(const float aDelta : myFrameValues)
    {
        total += aDelta;
    }

    return static_cast<float>(myFrameValues.size()) / total;
}

int CommonUtilities::TimerClock::GetFPS()
{
    return myFramesPerSecond;
}

float CommonUtilities::TimerClock::GetDeltaTime() const
{
    return myDeltaTime;
}

double CommonUtilities::TimerClock::GetTotalTime() const
{
    return myTotalTimeElapsed;
}

int CommonUtilities::TimerClock::GetElapsedFrames() const
{
    return myElapsedFrames - 1;
}