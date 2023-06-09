#include "Chattu.h"
#include "bot/Client.hpp"
#include "Commands/CumCmd.h"

Chattu::Chattu(Lucent::TwitchApi& client) : Bot(client)
{
	myCommands.push_back(std::make_shared<CumCmd>(this));
}

Chattu::~Chattu()
{
}

void Chattu::HandleEvent()
{
}

void Chattu::Update()
{
}

void Chattu::Draw()
{
}

void Chattu::HandlePRIVMSG(const Lucent::ChatMessage& priv, bool aIgnoreEnabledCheck)
{
	auto [first, second] = SplitCommand(priv.Message);

	for(int i = 0; i < myCommands.size(); i++)
	{
		if(myCommands[i]->IsCommand(first, priv.Message))
		{
			if(myCommands[i]->HandleCommand(myClient, priv, second))
			{
				return;
			}
		}
	}

	if(first == "!help")
	{
		SendPRIVMSG(priv.Channel, "Find me at https://github.com/maikatura/LucentOSC");
	}
}
