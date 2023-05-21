#pragma once
#include "bot/Command.h"


class VRChatMoveForwardCmd : public Command
{
public:
	VRChatMoveForwardCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
