#pragma once
#include <functional>


namespace TM
{
	typedef std::function<void()> TimerCallback;

	struct TimerHandle
	{
	private:
		int ID = -1;

	public:
		TimerHandle(unsigned int anId) { ID = anId; }
		[[nodiscard]] unsigned int GetID() const { return ID; }
		[[nodiscard]] bool IsValid() const { return ID >= 0; }
	};
}
