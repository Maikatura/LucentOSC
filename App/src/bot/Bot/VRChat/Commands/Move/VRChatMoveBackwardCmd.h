#pragma once
#include "bot/Command.h"


class VRChatMoveBackwardCmd : public Command
{
public:
	VRChatMoveBackwardCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
