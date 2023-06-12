#pragma once
#include "bot/Command.h"

class VRChatParameterPageCmd : public Command
{
public:
	VRChatParameterPageCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
