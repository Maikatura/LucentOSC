#pragma once
#include "bot/Command.h"


class VRChatBoolCmd : public Command
{
public:
	VRChatBoolCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
