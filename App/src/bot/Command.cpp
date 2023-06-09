#include "Command.h"

#include <iostream>

#include "Client.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "misc/print.h"
#include "misc/TimerManager.h"
#include "Twitch/TwitchApi.h"

Command::Command(Bot* aBot, const std::string& aCommandName, bool isARootCommand, bool aPrefixIsNeeded) :
	myBot(aBot), myCommandName(aCommandName), myIsRootCommand(isARootCommand), myPrefixNeeded(aPrefixIsNeeded)
{
}

bool Command::IsCommand(std::string aCommandName, std::string aFullMessage)
{
	if (!myUseRegex)
	{
		if(toLower(aCommandName) == toLower(myCommandName))
		{
			return true;
		}
	}
	else
	{
		std::regex reg(myRegexCommandCheck);
		if(std::regex_match(toLower(aFullMessage), reg))
		{
			return true;
		}
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
		if(mySubCommands[i]->IsCommand(first, priv.Message))
		{
			if (mySubCommands[i]->IsEnabled() && !mySubCommands[i]->IsOnCooldown())
			{
				if(mySubCommands[i]->HasSubCommands())
				{
					mySubCommands[i]->StartCooldown();
					return mySubCommands[i]->HandleCommand(aClient, priv, second);
				}

				mySubCommands[i]->StartCooldown();
				return mySubCommands[i]->HandleCommandLogic(aClient, priv, second);
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
			CommandDraw();

			if(!myIsEnabled)
			{
				ImGui::TreePop();
				return;
			}

			if(ImGui::TreeNode("Sub Commands"))
			{
				for(int i = 0; i < mySubCommands.size(); i++)
				{
					mySubCommands[i]->Draw();
					auto contextValue = mySubCommands[i]->DrawContextMenu(mySubCommands[i]->GetCommandName());

					switch(contextValue)
					{
						case ContextMenuReturn::Delete:
							mySubCommands.erase(mySubCommands.begin() + i);
							i--;
							break;
						case ContextMenuReturn::Copy:
							break;
						case ContextMenuReturn::Count:
							break;
						default: ;
					}
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
			CommandDraw();

			ImGui::TreePop();
		}
	}
}

ContextMenuReturn Command::DrawContextMenu(const std::string& aCommandName)
{
	ContextMenuReturn returnContext = ContextMenuReturn::None;
	std::string commandName = aCommandName + "##Popup";
	if(ImGui::BeginPopupContextItem(commandName.c_str()))
	{
		if(ImGui::Button("Delete"))
		{
			returnContext = ContextMenuReturn::Delete;
		}
		ImGui::EndPopup();
	}

	return returnContext;
}

void Command::DrawInternalStuff()
{
	ImGui::InputText("##name", &myCommandName);
	ImGui::Checkbox("Enabled", &myIsEnabled);

	if(!myIsEnabled)
	{
		return;
	}

	ImGui::Checkbox("Need Prefix", &myPrefixNeeded);
	ImGui::Checkbox("Use Regex", &myUseRegex);
	if (myUseRegex)
	{
		ImGui::InputText("Regex", &myRegexCommandCheck);
	}

	ImGui::InputFloat("Cooldown Time", &myCommandCooldownTime);
	if(ImGui::BeginCombo("##Type", CommandTriggerMap[myCommandTrigger].c_str()))
	{
		for (auto commandType : CommandTriggerMap)
		{
			if (commandType.first != CommandTrigger::Count)
			{
				bool is_selected = (myCommandTrigger == commandType.first);
				if(ImGui::Selectable(commandType.second.c_str(), is_selected))
				{
					myCommandTrigger = commandType.first;
					if(is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
		}
		ImGui::EndCombo();
	}
}

void Command::StartCooldown()
{
	if (!IsOnCooldown() && 0.0f < myCommandCooldownTime)
	{
		myIsOnCooldown = true;

		TimerManager::AddTimer([&]()
		{
			myIsOnCooldown = false;
		}, myCommandCooldownTime, false);
	}
	else
	{
		myIsOnCooldown = false;
	}
	
}

bool Command::NeedPrefix()
{
	return myPrefixNeeded;
}

bool Command::IsOnCooldown()
{
	return myIsOnCooldown;
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

void Command::SetIsRootCommand(bool isRootCommand)
{
	myIsRootCommand = isRootCommand;
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
