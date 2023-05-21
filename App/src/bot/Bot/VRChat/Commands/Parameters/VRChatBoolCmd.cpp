#include "VRChatBoolCmd.h"

#include "osclib/OSCData.h"
#include "bot/Bot/VRChat/VRChat.h"

VRChatBoolCmd::VRChatBoolCmd(Bot* aBot) : Command(aBot, "bool")
{ }

bool VRChatBoolCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{
	

	auto bot = GetBot<VRChat>();


	if(!IsAppOpen(L"VRChat"))
	{
		SendPRIVMSG(aClient, priv.Channel, "VRChat is not running");
		return true;
	}

	const auto [toggleName, toggleValue] = SplitCommand(aMessage);

	std::string parameterString = bot->GetFullParameterName(priv.Channel, toggleName, OSCType::Bool);

	if(parameterString == "INVALID")
	{
		SendPRIVMSG(aClient, priv.Channel, "That Parameter is not a bool");
		return true;
	}
	else if(!parameterString.empty())
	{
		if(toggleValue == "0" || toggleValue == "1" || toggleValue == "true" || toggleValue == "false")
		{
			if(toggleValue == "0" || toggleValue == "false")
			{
				bot->GetTransmitter().SendBool(parameterString, false);
			}
			else
			{
				bot->GetTransmitter().SendBool(parameterString, true);
			}

			return true;
		}
		else
		{
			SendPRIVMSG(aClient, priv.Channel, "Something is wrong, The valid values is: 0, false, 1 and true");
			return true;
		}
	}

	return false;
}
