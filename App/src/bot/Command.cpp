#include "Command.h"

#include <iostream>

#include "Client.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "misc/print.h"
#include "Twitch/TwitchApi.h"

Command::Command(Bot* aBot, const std::string& aCommandName, bool isARootCommand) : myBot(aBot), myCommandName(aCommandName), myIsRootCommand(isARootCommand)
{
}

bool Command::IsCommand(std::string aCommandName)
{
	if (toLower(aCommandName) == toLower(myCommandName))
	{
		return true;
	}

	return false;
}

bool Command::HandleCommandLogic(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& aMessage)
{
	return true;
}

bool Command::HandleCommand(Lucent::TwitchApi& aClient, const Lucent::ChatMessage& priv, const std::string& command)
{
	auto [first, second] = SplitCommand(command);
	for(int i = 0; i < mySubCommands.size(); i++)
	{
		if(mySubCommands[i]->IsCommand(first))
		{
			if (mySubCommands[i]->IsEnabled())
			{
				if(mySubCommands[i]->HasSubCommands())
				{
					return mySubCommands[i]->HandleCommand(aClient, priv, second);
				}
				else
				{
					return mySubCommands[i]->HandleCommandLogic(aClient, priv, second);
				}
			}
			
		}
	}

	return false;
}

std::pair<std::string, std::string> Command::SplitCommand(const std::string& command)
{
	std::string first, second;

	if(const std::size_t space = command.find(' '); space != std::string::npos)
	{
		first = command.substr(0, space);
		second = command.substr(space + 1);
	}
	else
	{
		first = command;
	}

	toLower(first);

	return { first, second };
}

void Command::SendPRIVMSG(Lucent::TwitchApi& aClient, const std::string& aChannel, const std::string& msg)
{
	aClient.SendChatMessage(aChannel, msg);
}

void Command::Draw()
{
	if (!mySubCommands.empty())
	{
		if(ImGui::TreeNode(myCommandName.c_str()))
		{
			DrawInternalStuff();

			if(ImGui::TreeNode("Sub Commands"))
			{
				for(int i = 0; i < mySubCommands.size(); i++)
				{
					mySubCommands[i]->Draw();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}
	else
	{
		if(ImGui::TreeNode(myCommandName.c_str()))
		{
			DrawInternalStuff();

			ImGui::TreePop();
		}
	}


}

void Command::DrawInternalStuff()
{
	ImGui::InputText("##name", &myCommandName);
	ImGui::Checkbox("Enabled", &myIsEnabled);
	if(ImGui::BeginCombo("##Type", CommandTriggerMap[myCommandTrigger].c_str())) // The second parameter is the label previewed before opening the combo.
	{
		for (auto commandType : CommandTriggerMap)
		{
			if (commandType.first != CommandTrigger::Count)
			{
				bool is_selected = (myCommandTrigger == commandType.first); // You can store your selection however you want, outside or inside your objects
				if(ImGui::Selectable(commandType.second.c_str(), is_selected))
				{
					myCommandTrigger = commandType.first;
					if(is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
					// You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
			}
		}
		ImGui::EndCombo();
	}
}

bool Command::HasSubCommands()
{
	return !mySubCommands.empty();
}

bool& Command::IsEnabled()
{
	return myIsEnabled;
}

bool Command::IsRootCommand()
{
	return myIsRootCommand;
}

std::string Command::GetCommandName()
{
	return myCommandName;
}

bool Command::IsAppOpen(const std::wstring& aApplication)
{
	if(!IsProgramRunning(L"VRChat"))
	{
		return false;
	}

	return true;
}
