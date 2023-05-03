#include "VRChatBoolCmd.h"

#include "osclib/OSCData.h"
#include "bot/Bot/VRChat/VRChat.h"

VRChatBoolCmd::VRChatBoolCmd(Bot* aBot, const std::string& aCommandName) : Command(aBot, aCommandName)
{ }

bool VRChatBoolCmd::HandleCommandLogic(Client& aClient, const PRIVMSG& priv, const std::string& aMessage)
{
	const auto [commandType, commandData] = SplitCommand(aMessage);


	auto bot = GetBot<VRChat>();


	if(!IsAppOpen(L"VRChat"))
	{
		SendPRIVMSG(aClient, priv.Channel, "VRChat is not running");
		return true;
	}

	const auto [toggleName, toggleValue] = SplitCommand(commandData);

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
