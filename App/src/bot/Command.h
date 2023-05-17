#pragma once
#include <string>
#include <vector>

#include "Utility.hpp"
#include <utility>

#include <Testing/TwitchApi.h>

class Bot;

namespace Lucent
{
	class TwitchApi;
	struct ChatMessage;
}

class Command
{
public:


	Command(Bot* aBot, const std::string& aCommandName, bool isARootCommand = false);
	bool IsCommand(std::string aCommandName);

	virtual bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage);
	bool HandleCommand(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& command);
	std::pair<std::string, std::string> SplitCommand(const std::string& command);

	void SendPRIVMSG(Lucent::TwitchApi& aClient, const std::string& aChannel, const std::string& msg);

	template<typename T>
	T* GetBot();

	bool HasSubCommands();
	bool IsEnabled();
	bool IsRootCommand();

protected:

	bool IsAppOpen(const std::wstring& aApplication);

	std::string myCommandName;
	std::vector<std::shared_ptr<Command>> mySubCommands;

	bool myIsEnabled = true;
	bool myIsRootCommand = false;

private:
	Bot* myBot = nullptr;
};

template <typename T>
T* Command::GetBot()
{
	return reinterpret_cast<T*>(myBot);
}


