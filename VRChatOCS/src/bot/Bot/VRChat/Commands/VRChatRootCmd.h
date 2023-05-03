#pragma once
#include "bot/Command.h"

class VRChatRootCmd : public Command
{
public:
	VRChatRootCmd(Bot* aBot);
	bool HandleCommandLogic(Client& aClient,const PRIVMSG& priv, const std::string& aMessage) override;
};
