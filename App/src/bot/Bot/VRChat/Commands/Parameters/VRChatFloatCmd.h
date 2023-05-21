#pragma once
#include "bot/Command.h"


class VRChatFloatCmd : public Command
{
public:
	VRChatFloatCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
