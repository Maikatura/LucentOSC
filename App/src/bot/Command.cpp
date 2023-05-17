#include "Command.h"

#include <iostream>

#include "Client.hpp"
#include "misc/print.h"
#include "Testing/TwitchApi.h"

Command::Command(Bot* aBot, const std::string& aCommandName) : myBot(aBot), myCommandName(aCommandName)
{
}

bool Command::IsCommand(std::string aCommandName)
{
	if (toLower(aCommandName) == toLower(myCommandName))
	{
		return true;
	}

	return false;
}

bool Command::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{
	return false;
}

bool Command::HandleCommand(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& command)
{
	auto [first, second] = SplitCommand(command);
	for(int i = 0; i < mySubCommands.size(); i++)
	{
		if(mySubCommands[i]->IsCommand(first))
		{
			if(mySubCommands[i]->HandleCommand(aClient, priv, second))
			{
				return true;
			}
		}
	}

	return HandleCommandLogic(aClient, priv, command);
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

void Command::SendPRIVMSG(Lucent::TwitchApi& aClient, const std::string& aChannel, const std::string& msg)
{
	aClient.SendChatMessage(aChannel, msg);
}

bool Command::IsEnabled()
{
	return myIsEnabled;
}

bool Command::IsAppOpen(const std::wstring& aApplication)
{
	if(!IsProgramRunning(L"VRChat"))
	{
		return false;
	}

	return true;
}
