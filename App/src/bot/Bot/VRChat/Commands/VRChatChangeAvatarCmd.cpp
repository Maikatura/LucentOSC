#include "VRChatChangeAvatarCmd.h"

VRChatChangeAvatarCmd::VRChatChangeAvatarCmd(Bot* aBot) : Command(aBot, "AvatarChange")
{
}

bool VRChatChangeAvatarCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
                                                const std::string& aMessage) 
{
	std::string avatarName;

	if(myOldAvatar.empty())
	{
		aClient.SendChatMessage(priv.Channel, "Changed into '" + avatarName + "'");
	}
	else
	{
		aClient.SendChatMessage(priv.Channel, "Changed from '" + myOldAvatar + "' to '"+ avatarName + "'");
	}

	myOldAvatar = avatarName;

	return true;
}
