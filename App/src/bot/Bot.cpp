#include "Bot.hpp"
#include "Client.hpp"
#include "Utility.hpp"

Bot::Bot(Lucent::TwitchApi& client)
	: myClient(client)
{
}

void Bot::HandleBotCommands(Lucent::ChatMessage& priv)
{
	HandleCommands(priv);
	HandlePRIVMSG(priv);
}

void Bot::HandleCommands(Lucent::ChatMessage& priv)
{

	int indexPos = 0;
	while(indexPos < priv.Message.length())
	{
		indexPos = priv.Message.find('\r\n', indexPos);
		if(indexPos != std::string::npos)
		{
			priv.Message.erase(indexPos);
		}
	}

	for(int i = 0; i < myCommands.size(); i++)
	{
		auto [first, second] = SplitCommand(priv.Message);

		myCommands[i]->SetIsRootCommand(true);

		if (myCommands[i]->NeedPrefix())
		{
			first.erase(0, 1);
		}

		if(myCommands[i]->IsCommand(first, priv.Message))
		{
			if(myCommands[i]->HasSubCommands() && myCommands[i]->IsEnabled() && !myCommands[i]->IsOnCooldown())
			{
				myCommands[i]->StartCooldown();
				myCommands[i]->HandleCommand(myClient, priv, second);
			}
			else if (myCommands[i]->IsEnabled() && !myCommands[i]->IsOnCooldown())
			{
				myCommands[i]->HandleCommandLogic(myClient, priv, second);
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
