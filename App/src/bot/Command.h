#pragma once
#include <string>
#include <vector>

#include "Utility.hpp"
#include <utility>

#include "Client.hpp"

class Client;
class Bot;

class Command
{
public:


	Command(Bot* aBot, const std::string& aCommandName);
	bool IsCommand(std::string aCommandName);

	virtual bool HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage);
	bool HandleCommand(Client& aClient, const PRIVMSG& priv, const std::string& command);
	std::pair<std::string, std::string> SplitCommand(const std::string& command);

	void SendPRIVMSG(Client& aClient, const std::string& aChannel, const std::string& msg);

	template<typename T>
	T* GetBot();

protected:

	bool IsAppOpen(const std::wstring& aApplication);

	std::string myCommandName;
	std::vector<std::shared_ptr<Command>> mySubCommands;

private:
	Bot* myBot;
};

template <typename T>
T* Command::GetBot()
{
	return reinterpret_cast<T*>(myBot);
}


