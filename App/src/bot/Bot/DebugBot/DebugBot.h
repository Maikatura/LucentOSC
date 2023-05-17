#pragma once

#include <filesystem>

#include "Bot/Bot.hpp"

#include <queue>
#include <thread>
#include <mutex>

#include "osclib/osc.h"
#include "osclib/OSCData.h"

class DebugBot : public Bot
{

public:
	explicit DebugBot(Lucent::TwitchApi& client);
	~DebugBot();

	void HandleEvent() override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const Lucent::ChatMessage& priv) override;
private:
};
