#include "VRChatLookLeftCmd.h"

#include "bot/Bot/VRChat/VRChat.h"
#include "misc/TimerManager.h"

VRChatLookLeftCmd::VRChatLookLeftCmd(Bot* aBot) : Command(aBot, "left")
{
}

bool VRChatLookLeftCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{

	auto bot = GetBot<VRChat>();

	if(!IsAppOpen(L"VRChat")) return false;


	bot->GetTransmitter().SendInt("/input/LookLeft", 1);

	std::string value = aMessage;
	if(aMessage.empty() && ContainsOnlyNumber(aMessage) || AllSameChars(aMessage, '.'))
	{
		value = "1";
	}

	TimerManager::AddTimer([&]()
		{
			GetBot<VRChat>()->GetTransmitter().SendInt("/input/LookLeft", 0);
		}, std::stof(value), false);

	return true;
}
