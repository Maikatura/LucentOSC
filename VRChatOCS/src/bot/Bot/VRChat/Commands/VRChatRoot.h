#pragma once
#include "bot/Command.h"

class VRChatRoot : public Command
{
public:
	VRChatRoot(const std::string& aCommandName);
	bool HandleCommandLogic(Client& aClient,const PRIVMSG& priv) override;
};
