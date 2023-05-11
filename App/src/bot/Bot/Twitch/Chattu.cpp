#include "Chattu.h"
#include "bot/Client.hpp"
#include "Commands/CumCmd.h"

Chattu::Chattu(Client& client) : Bot(client)
{
	myCommands.push_back(std::make_shared<CumCmd>(this));
}

Chattu::~Chattu()
{
}

void Chattu::HandleEvent(const sf::Event& event)
{
}

void Chattu::Update()
{
}

void Chattu::Draw()
{
}

void Chattu::HandlePRIVMSG(const PRIVMSG& priv)
{
	auto [first, second] = SplitCommand(priv.message);
	first.erase(0, 1);

	for(int i = 0; i < myCommands.size(); i++)
	{
		if(myCommands[i]->IsCommand(first))
		{
			if(myCommands[i]->HandleCommand(myClient, priv, second))
			{
				return;
			}
		}
	}

	if(first == "!help")
	{
		SendPRIVMSG(priv.Channel, "Find me at https://github.com/maikatura/VRCBotTV");
	}
}
