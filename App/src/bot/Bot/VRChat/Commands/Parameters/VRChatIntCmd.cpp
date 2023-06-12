#include "VRChatIntCmd.h"

#include "bot/Bot/VRChat/VRChat.h"
#include "osclib/OSCData.h"

VRChatIntCmd::VRChatIntCmd(Bot* aBot) : Command(aBot, "int")
{
}

bool VRChatIntCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{

	if(!IsAppOpen(L"VRChat")) return false;


	auto bot = GetBot<VRChat>();

	const auto [toggleName, toggleValue] = SplitCommand(aMessage);

	if(toggleValue.empty() || ContainsOnlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
	{
		SendPRIVMSG(aClient, priv.Channel, "Value is Null or Invalid Input.");
		return true;
	}

	int value = std::stoi(toggleValue);

	std::string parameterString = bot->GetFullParameterName(priv.Channel, toggleName, OSCType::Int);
	if(parameterString == "INVALID")
	{
		SendPRIVMSG(aClient, priv.Channel, "That Parameter is not a int");
	}
	else if(!parameterString.empty())
	{
		if(value >= 0 && value <= 256)
		{
			GetBot<VRChat>()->GetTransmitter().SendInt(parameterString, value);
		}
	}

	return true;
}
