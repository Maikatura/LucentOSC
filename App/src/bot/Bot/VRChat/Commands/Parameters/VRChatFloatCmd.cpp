#include "VRChatFloatCmd.h"

#include "osclib/OSCData.h"
#include "bot/Bot/VRChat/VRChat.h"




VRChatFloatCmd::VRChatFloatCmd(Bot* aBot) : Command(aBot, "float")
{ }

bool VRChatFloatCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{
	auto bot = GetBot<VRChat>();


	if(!IsAppOpen(L"VRChat"))
	{
		SendPRIVMSG(aClient, priv.Channel, "VRChat is not running");
		return true;
	}

	const auto [toggleName, toggleValue] = SplitCommand(aMessage);

	if(toggleValue.empty() || ContainsOnlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
	{
		SendPRIVMSG(aClient, priv.Channel, "Value is Null or Invalid Input.");
		return false;
	}

	std::string parameterString = bot->GetFullParameterName(priv.Channel, toggleName, OSCType::Float);
	float value = std::stof(toggleValue);


	if(parameterString == "INVALID")
	{
		SendPRIVMSG(aClient, priv.Channel, "That Parameter is not a bool");
		return true;
	}
	else if(!parameterString.empty())
	{
		if(value >= 0.0f && value <= 1.0f)
		{
			bot->GetTransmitter().SendFloat(parameterString, value);
			return true;
		}
	}

	return false;
}
