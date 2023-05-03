#include "VRChatRootCmd.h"

// Commands
#include "VRChatBoolCmd.h"


VRChatRootCmd::VRChatRootCmd(Bot* aBot, const std::string& aCommandName) : Command(aBot, aCommandName)
{
	mySubCommands.push_back(std::make_shared<VRChatBoolCmd>(aBot, "bool"));
}

bool VRChatRootCmd::HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage)
{
	SendPRIVMSG(aClient, priv.Channel, "Lmao this is the root");

	return false;
}
