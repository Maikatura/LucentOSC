#include "VRChatRootCmd.h"

// Commands
#include "VRChatBoolCmd.h"


VRChatRootCmd::VRChatRootCmd(Bot* aBot) : Command(aBot, "vrchat")
{
	mySubCommands.push_back(std::make_shared<VRChatBoolCmd>(aBot));
}

bool VRChatRootCmd::HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage)
{
	//SendPRIVMSG(aClient, priv.Channel, "I don't know what should happen here");

	return false;
}
