#pragma once
#include "bot/Command.h"


class CumCmd : public Command
{
public:
	CumCmd(Bot* aBot);
	bool HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage) override;
};
