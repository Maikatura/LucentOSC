#pragma once
#include "bot/Command.h"

class VRChatLookRightCmd : public Command
{
public:
	VRChatLookRightCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
