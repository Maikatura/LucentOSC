#include "Bot.hpp"
#include "Client.hpp"
#include "Utility.hpp"

Bot::Bot(Lucent::TwitchApi& client)
	: myClient(client)
{
}

void Bot::HandleBotCommands(const Lucent::ChatMessage& priv)
{
	HandleCommands(priv);
	HandlePRIVMSG(priv);
}

void Bot::HandleCommands(const Lucent::ChatMessage& priv)
{
	auto [first, second] = SplitCommand(priv.Message);
	for(int i = 0; i < myCommands.size(); i++)
	{
		if(myCommands[i]->IsCommand(first))
		{
			if(myCommands[i]->HasSubCommands() && myCommands[i]->IsEnabled() && !myCommands[i]->IsOnCooldown())
			{
				myCommands[i]->StartCooldown();
				myCommands[i]->HandleCommand(myClient, priv, second);
			}
		}
	}
}


bool Bot::IsAdmin(const std::string& username) const
{
	return false;
}

void Bot::SendPRIVMSG(const std::string& aChannel, const std::string& msg)
{
	myClient.SendChatMessage(aChannel, msg);
}

std::pair<std::string, std::string> SplitCommand(const std::string& command)
{
	std::string first, second;

	if (const std::size_t space = command.find(' '); space != std::string::npos)
	{
		first = command.substr(0, space);
		second = command.substr(space + 1);
	}
	else
	{
		first = command;
	}

	toLower(first);

	return { first, second };
}
