#include "TimerManager.h"


TimerManager::TimerManager()
{
}


void TimerManager::Update()
{
	myHeartbeatTimer.Update();

	auto It = myTimers.begin();
	while (It != myTimers.end())
	{
		It->second.TimeElapsed += myHeartbeatTimer.GetDeltaTime();
		if(It->second.TimeElapsed >= It->second.Duration)
		{
			It->second.Callback();
			if(!It->second.IsRepeating)
			{
				It->second.Callback = nullptr;
				It = myTimers.erase(It);
				continue;
			}
			It->second.TimeElapsed -= It->second.Duration;
		}
		++It;
	}
}

TM::TimerHandle TimerManager::AddTimer(TM::TimerCallback aCallback, float aDuration, bool shouldLoop)
{
	ManagedTimer timer;
	timer.Callback = aCallback;
	timer.Duration = aDuration;
	timer.IsRepeating = shouldLoop;
	timer.TimeElapsed = 0.0f;

	myTimers[myIdCounter] = timer;

	TM::TimerHandle handle = TM::TimerHandle(myIdCounter);
	myIdCounter++;

	return handle;
}

bool TimerManager::ClearTimer(TM::TimerHandle& aHandle)
{
	auto It = myTimers.begin();
	while(It != myTimers.end())
	{
		if(It->first == aHandle.GetID())
		{
			It = myTimers.erase(It);
			return true;
		}
	}

	return false;
}
