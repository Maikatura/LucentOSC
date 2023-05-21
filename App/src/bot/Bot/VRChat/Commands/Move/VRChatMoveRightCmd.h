#pragma once
#include "bot/Command.h"


class VRChatMoveRightCmd : public Command
{
public:
	VRChatMoveRightCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
