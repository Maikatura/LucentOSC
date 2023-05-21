#include "VRChatRootCmd.h"

// Commands
#include "Parameters/VRChatBoolCmd.h"
#include "Parameters/VRChatFloatCmd.h"
#include "VRChatMoveCmd.h"


VRChatRootCmd::VRChatRootCmd(Bot* aBot) : Command(aBot, "vrchat")
{
	mySubCommands.push_back(std::make_shared<VRChatBoolCmd>(aBot));
	mySubCommands.push_back(std::make_shared<VRChatFloatCmd>(aBot));



	mySubCommands.push_back(std::make_shared<VRChatMoveCmd>(aBot));

}

bool VRChatRootCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{
	//SendPRIVMSG(aClient, priv.Channel, "I don't know what should happen here");

	return false;
}
