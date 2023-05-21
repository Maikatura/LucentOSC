#pragma once
#include "bot/Command.h"


class VRChatChangeAvatarCmd : public Command
{
public:
	VRChatChangeAvatarCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;

private:
	std::string myOldAvatar;
};
