#pragma once
#include "bot/Command.h"


class VRChatLookCmd : public Command
{
public:
	VRChatLookCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
