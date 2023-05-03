#include "VRChatRoot.h"


VRChatRoot::VRChatRoot(const std::string& aCommandName) : Command(aCommandName)
{
}

bool VRChatRoot::HandleCommandLogic(Client& aClient, const PRIVMSG& priv)
{
	SendPRIVMSG(aClient, priv.Channel, "Lmao this is the root");

	return true;
}
