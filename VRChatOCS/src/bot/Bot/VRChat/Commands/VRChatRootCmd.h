#pragma once
#include "bot/Command.h"

class VRChatRootCmd : public Command
{
public:
	VRChatRootCmd(Bot* aBot, const std::string& aCommandName);
	bool HandleCommandLogic(Client& aClient,const PRIVMSG& priv, const std::string& aMessage) override;
};
