#pragma once
#include "bot/Command.h"


class VRChatMoveLeftCmd : public Command
{
public:
	VRChatMoveLeftCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
