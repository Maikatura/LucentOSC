#pragma once
#include "bot/Command.h"


class CumCmd : public Command
{
public:
	CumCmd(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;
};
