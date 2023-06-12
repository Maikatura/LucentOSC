#include "VRChatRootCmd.h"

// Commands
#include "VRChatJumpCmd.h"
#include "VRChatLookCmd.h"
#include "Parameters/VRChatBoolCmd.h"
#include "Parameters/VRChatFloatCmd.h"
#include "VRChatMoveCmd.h"
#include "VRChatParameterPageCmd.h"
#include "VRChatSpeakCmd.h"
#include "Parameters/VRChatIntCmd.h"


VRChatRootCmd::VRChatRootCmd(Bot* aBot) : Command(aBot, "vrchat")
{

	mySubCommands.push_back(std::make_shared<VRChatBoolCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatFloatCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatIntCmd>(aBot));

	mySubCommands.push_back(std::make_shared<VRChatSpeakCmd>(aBot));

	mySubCommands.push_back(std::make_shared<VRChatMoveCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatLookCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatJumpCmd>(aBot));

	mySubCommands.push_back(std::make_shared<VRChatParameterPageCmd>(aBot));
}

bool VRChatRootCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{
	//SendPRIVMSG(aClient, priv.Channel, "I don't know what should happen here");

	return false;
}
