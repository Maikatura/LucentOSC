#include "VRChatParameterPageCmd.h"

#include "bot/Bot/VRChat/VRChat.h"

VRChatParameterPageCmd::VRChatParameterPageCmd(Bot* aBot) : Command(aBot, "page")
{
}

bool VRChatParameterPageCmd::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv,
	const std::string& aMessage)
{
	if(!IsAppOpen(L"VRChat")) return false;


	auto[pageString, other] = SplitCommand(aMessage);

	int page;
	if(pageString.empty())
	{
		page = 1;
	}
	else
	{
		page = std::stoi(pageString);
	}

	page = 5 * page - 1;

	if(page < 0)
	{
		page = 0;
	}

	auto bot = GetBot<VRChat>();

	std::string pageParameters = "Page ";
	pageParameters += pageString;
	pageParameters += ": ";

	for(int i = 0; i < 5; i++)
	{
		if(page + i <= bot->GetAvatarData().Parameters.size())
		{
			if(i == 4)
			{
				pageParameters += bot->GetAvatarData().Parameters[page + i].ParameterName;
			}
			else
			{
				pageParameters += bot->GetAvatarData().Parameters[page + i].ParameterName + ", ";
			}
		}
		else
		{
			int pageRightIndex = page;
			while (pageRightIndex >= bot->GetAvatarData().Parameters.size() - 5)
			{
				pageRightIndex--;
			}

			if(i == 4)
			{
				pageParameters += bot->GetAvatarData().Parameters[pageRightIndex + i].ParameterName;
			}
			else
			{
				pageParameters += bot->GetAvatarData().Parameters[pageRightIndex + i].ParameterName + ", ";
			}
		}
	}

	SendPRIVMSG(aClient, priv.Channel, pageParameters);
	return true;
}
