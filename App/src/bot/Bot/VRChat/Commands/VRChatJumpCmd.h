#pragma once
#include "bot/Command.h"


class VRChatJumpCmd : public Command
{
public:
	VRChatJumpCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
