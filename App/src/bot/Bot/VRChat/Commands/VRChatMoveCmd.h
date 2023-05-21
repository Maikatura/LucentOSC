#pragma once
#include "bot/Command.h"


class VRChatMoveCmd : public Command
{
public:
	VRChatMoveCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
