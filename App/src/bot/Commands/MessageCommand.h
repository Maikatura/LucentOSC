#pragma once
#include "bot/Command.h"

class MessageCommand : public Command
{
public:
	MessageCommand(Bot* aBot);
	bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage) override;

	void CommandDraw() override;

private:
	std::string myCommandMessage;
};
