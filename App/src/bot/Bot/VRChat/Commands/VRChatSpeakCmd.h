#pragma once
#include "bot/Command.h"

class VRChatSpeakCmd : public Command
{
public:
	VRChatSpeakCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
