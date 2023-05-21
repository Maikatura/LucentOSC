#pragma once


#include <memory>

#include "Command.h"
#include "Bot/Discord/Base/CommandBase.h"


namespace Lucent
{
	class TwitchApi;
	struct ChatMessage;
}


inline bool ContainsOnlyNumber(std::string const& str) {
	return str.find_first_of("1234567890.") ==
		std::string::npos;
}

inline bool AllSameChars(std::string testStr, char aChar) {
	return testStr.find_first_not_of(aChar) == std::string::npos;
}

class Bot
{
public:
	using Ptr = std::unique_ptr<Bot>;

public:
	explicit Bot(Lucent::TwitchApi& client);
	virtual ~Bot() = default;

	Bot(const Bot&) = delete;
	Bot& operator=(const Bot&) = delete;

	virtual void HandleEvent() = 0;
	virtual void Update() {};
	virtual void Draw() = 0;


	void HandleBotCommands(const Lucent::ChatMessage& priv);

	void HandleCommands(const Lucent::ChatMessage& priv);

	virtual void HandlePRIVMSG(const Lucent::ChatMessage& aMessage, bool aIgnoreEnabledCheck = false) = 0;

protected:
	bool IsAdmin(const std::string& username) const;
	void SendPRIVMSG(const std::string& aChannel, const std::string& msg);

	std::vector<std::shared_ptr<Command>> myCommands;

	Lucent::TwitchApi& myClient;
private:
};

std::pair<std::string, std::string> SplitCommand(const std::string& command);
