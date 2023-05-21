#include "VRChatMoveCmd.h"

#include "Move/VRChatMoveBackwardCmd.h"
#include "Move/VRChatMoveForwardCmd.h"
#include "Move/VRChatMoveLeftCmd.h"
#include "Move/VRChatMoveRightCmd.h"

VRChatMoveCmd::VRChatMoveCmd(Bot* aBot) : Command(aBot, "move")
{
	mySubCommands.push_back(std::make_shared<VRChatMoveLeftCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatMoveRightCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatMoveForwardCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatMoveBackwardCmd>(aBot));
}

bool VRChatMoveCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{
	return Command::HandleCommandLogic(aClient, priv, aMessage);
}
