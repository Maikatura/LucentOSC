#include "VRChatJumpCmd.h"

#include "bot/Bot/VRChat/VRChat.h"
#include "misc/TimerManager.h"

VRChatJumpCmd::VRChatJumpCmd(Bot* aBot) : Command(aBot, "jump")
{
}

bool VRChatJumpCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{
	if(!IsAppOpen(L"VRChat")) return false;

	auto bot = GetBot<VRChat>();
	bot->GetTransmitter().SendInt("/input/Jump", 1);

	TimerManager::AddTimer([&]()
		{
			GetBot<VRChat>()->GetTransmitter().SendInt("/input/Jump", 0);
		}, 0.1f, false);

	return true;
}
