#include "VRChatSpeakCmd.h"

#include "bot/Bot/VRChat/VRChat.h"


VRChatSpeakCmd::VRChatSpeakCmd(Bot* aBot) : Command(aBot, "speak")
{
	
}

bool VRChatSpeakCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{

	auto bot = GetBot<VRChat>();

	if(!IsAppOpen(L"VRChat")) return false;



	if(priv.IsModerator)
	{
		bot->GetTransmitter().SendChatboxMessage("/chatbox/input", aMessage);
	}
	else
	{
		SendPRIVMSG(aClient, priv.Channel, "You can't use that command (Moderator Only)");
	}

	return true;
}
