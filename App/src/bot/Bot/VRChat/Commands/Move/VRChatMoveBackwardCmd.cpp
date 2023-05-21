#include "VRChatMoveBackwardCmd.h"

#include "bot/Bot/VRChat/VRChat.h"
#include "misc/TimerManager.h"

VRChatMoveBackwardCmd::VRChatMoveBackwardCmd(Bot* aBot) : Command(aBot, "backward")
{
}

bool VRChatMoveBackwardCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{
	if(!IsAppOpen(L"VRChat"))
		return false;


	auto bot = GetBot<VRChat>();

	const auto toggleValue = aMessage;

	bot->GetTransmitter().SendInt("/input/MoveBackward", 1);

	std::string value = toggleValue;
	if(toggleValue.empty() && ContainsOnlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
	{
		value = "1";
	}

	TimerManager::AddTimer([&]()
	{
		GetBot<VRChat>()->GetTransmitter().SendInt("/input/MoveBackward", 0);
	}, std::stof(value), false);

	return true;
}
