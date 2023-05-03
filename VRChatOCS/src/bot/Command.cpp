#include "Command.h"

#include "Client.hpp"

Command::Command(const std::string& aCommandName) : myCommandName(aCommandName)
{ }

bool Command::IsCommand(std::string aCommandName)
{
	if (toLower(aCommandName) == toLower(myCommandName))
	{
		return true;
	}

	return false;
}

bool Command::HandleCommandLogic(Client& aClient, const PRIVMSG& priv)
{
	return false;
}

bool Command::HandleCommand(Client& aClient, const PRIVMSG& priv)
{
	for (int i = 0; i < mySubCommands.size(); i++)
	{
		if (mySubCommands[i]->HandleCommand(aClient, priv))
		{
			return true;
		}
	}

	HandleCommandLogic(aClient, priv);

	return false;
}

std::pair<std::string, std::string> Command::SplitCommand(const std::string& command)
{
	std::string first, second;

	if(const std::size_t space = command.find(' '); space != std::string::npos)
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

void Command::SendPRIVMSG(Client& aClient, const std::string& aChannel, const std::string& msg)
{
	aClient.sendPRIVMSG(msg, aChannel);
}
