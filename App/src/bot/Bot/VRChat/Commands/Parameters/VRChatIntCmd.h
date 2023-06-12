#pragma once
#include "bot/Command.h"


class VRChatIntCmd : public Command
{
public:
	VRChatIntCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
