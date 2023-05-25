#pragma once
#include <string>
#include <vector>

#include "Utility.hpp"
#include <utility>

#include <Twitch/TwitchApi.h>
#include <json/json.hpp>
#include <type_traits>

#include "misc/EnumHelper.h"
#include <regex>

class Bot;

namespace Lucent
{
	class TwitchApi;
	struct ChatMessage;
}



enum class CommandTrigger
{
	Chat,
	Subscriber,
	Bits,
	GiftedSubs,
	ChannelPoints,
	Count
};

inline std::map<CommandTrigger, std::string> CommandTriggerMap = {
	{ CommandTrigger::Chat, "Chat" },
	{ CommandTrigger::Subscriber, "Subscriber" },
	{ CommandTrigger::Bits, "Bits" },
	{ CommandTrigger::GiftedSubs, "Gifted Subs" },
	{ CommandTrigger::ChannelPoints, "Channel Points" },
	{ CommandTrigger::Count, "Count" }
};

enum class ContextMenuReturn
{
	None,
	Delete,
	Copy,
	Count,
};

class Command
{



public:
	Command(Bot* aBot, const std::string& aCommandName, bool isARootCommand = false, bool aPrefixIsNeeded = true);
	bool IsCommand(std::string aCommandName, std::string aFullMessage);

	virtual bool HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage);
	bool HandleCommand(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& command);
	std::pair<std::string, std::string> SplitCommand(const std::string& command);

	void SendPRIVMSG(Lucent::TwitchApi& aClient, const std::string& aChannel, const std::string& msg);


	void Draw();
	ContextMenuReturn DrawContextMenu(const std::string& aCommandName);
	virtual void CommandDraw() {}


	template<typename T>
	T* GetBot();

	void StartCooldown();

	bool NeedPrefix();
	bool IsOnCooldown();
	bool HasSubCommands();
	bool IsRootCommand();
	void SetIsRootCommand(bool isRootCommand);;
	bool& IsEnabled();
	std::string GetCommandName();

protected:
	bool IsAppOpen(const std::wstring& aApplication);


	bool myUseRegex = false;
	bool myIsEnabled = true;
	bool myIsRootCommand = false;
	bool myPrefixNeeded = false;
	bool myIsOnCooldown = false;

	int myBitAmount = 0;
	int mySubscriberMonths = 0;

	float myCommandCooldownTime = 0.0f;

	CommandTrigger myCommandTrigger = CommandTrigger::Chat;

	std::string myCommandName;
	std::vector<std::shared_ptr<Command>> mySubCommands;

	std::string myRegexCommandCheck;

private:

	void DrawInternalStuff();

	Bot* myBot = nullptr;
};


template <typename T>
T* Command::GetBot()
{
	return reinterpret_cast<T*>(myBot);
}


