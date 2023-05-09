#pragma once
#include "bot/Command.h"


class VRChatBoolCmd : public Command
{
public:
	VRChatBoolCmd(Bot* aBot);
	bool HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage) override;
};
