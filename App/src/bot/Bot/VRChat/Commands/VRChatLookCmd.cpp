#include "VRChatLookCmd.h"

#include "bot/Bot/VRChat/VRChat.h"
#include "bot/bot/VRChat/Commands/Look/VRChatLookLeftCmd.h"
#include "bot/bot/VRChat/Commands/Look/VRChatLookRightCmd.h"

VRChatLookCmd::VRChatLookCmd(Bot* aBot) : Command(aBot, "look")
{
	mySubCommands.push_back(std::make_shared<VRChatLookLeftCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatLookRightCmd>(aBot));
}

bool VRChatLookCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{
	return Command::HandleCommandLogic(aClient, priv, aMessage);
}
