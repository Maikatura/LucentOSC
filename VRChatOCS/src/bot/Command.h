#pragma once
#include <string>
#include <vector>

#include "Utility.hpp"
#include <utility>

#include "Client.hpp"

class Client;

class Command
{
public:

	Command(const std::string& aCommandName);
	bool IsCommand(std::string aCommandName);

	virtual bool HandleCommandLogic(Client& aClient, const PRIVMSG& priv);
	bool HandleCommand(Client& aClient, const PRIVMSG& priv);
	std::pair<std::string, std::string> SplitCommand(const std::string& command);

	void SendPRIVMSG(Client& aClient, const std::string& aChannel, const std::string& msg);

private:
	std::string myCommandName;
	std::vector<std::shared_ptr<Command>> mySubCommands;
};


