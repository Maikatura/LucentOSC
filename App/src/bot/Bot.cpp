#include "Bot.hpp"
#include "Client.hpp"
#include "Utility.hpp"

Bot::Bot(Lucent::TwitchApi& client)
	: myClient(client)
{
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
