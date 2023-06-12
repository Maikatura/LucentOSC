#pragma once
#include "bot/Command.h"

class VRChatLookLeftCmd : public Command
{
public:
	VRChatLookLeftCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
