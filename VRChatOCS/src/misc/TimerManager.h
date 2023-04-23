#pragma once
#include <functional>
#include <memory>

#include "TimerClock.h"
#include "TimerHandle.h"


	class TimerManager
	{
	private:
		struct ManagedTimer
		{
			TM::TimerCallback Callback;
			float TimeElapsed;
			float Duration;
			bool IsRepeating = false;
		};

		
		inline static std::unordered_map<unsigned int, ManagedTimer> myTimers;
		inline static CommonUtilities::TimerClock myHeartbeatTimer;
		inline static unsigned int myIdCounter = 0;

	public:

		TimerManager();


		static void Update();
		static TM::TimerHandle AddTimer(TM::TimerCallback aCallback, float aDuration, bool shouldLoop = false);
		static bool ClearTimer(TM::TimerHandle& aHandle);
	};

