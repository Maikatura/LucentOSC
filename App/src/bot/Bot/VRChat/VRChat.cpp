#include "VRChat.h"
#include <iostream>

#include "imgui.h"
#include "bot/Client.hpp"
#include "bot/Commands/MessageCommand.h"
#include "Commands/VRChatRootCmd.h"
#include "misc/print.h"
#include "misc/TimerManager.h"
#include "osclib/sock.h"

inline bool ContainsOlyNumber(std::string const& str) {
	return str.find_first_of("1234567890.") ==
		std::string::npos;
}

VRChat::VRChat(Lucent::TwitchApi& client) : ::Bot(client)
{
	sock::Startup();

	std::cout << std::endl;

	if(!IsProgramRunning(L"VRChat"))
	{
		std::cout << "Please start vrchat and then press any key" << std::endl;
	}

	std::cout << "Don't forget to Enable OCS in the radial menu." << std::endl;

	myOSCListener.received = [](char const* ptr, int len)
	{
		print_hex_dump(ptr, len);
	};

	myOSCListener.value = [&](std::string const& addr, osc::Value const& value)
	{
		if (addr == "/avatar/change")
		{
			std::filesystem::path foundPath;
			std::vector<std::string> files;
			files.push_back(value.GetStringValue() + ".json");

			std::string path = std::getenv("appdata");
			path += "\\..\\LocalLow\\VRChat\\VRChat\\OSC\\";

			if (FindFile(path, foundPath, value.GetStringValue() + ".json"))
			{
				std::string oldAvatarName = myAvatarData.AvatarName;
				LoadData(myAvatarData, foundPath.string());

				if (oldAvatarName.empty())
				{
					
					SendPRIVMSG("#maikatura", "Changed into '" + myAvatarData.AvatarName + "'");
				}
				else
				{
					SendPRIVMSG("#maikatura", "Changed avatar from '"+ oldAvatarName+ "' to '" + myAvatarData.AvatarName + "'");
				}
			}
		}

		//print_osc_value(addr, value);
	};

	auto commandRoot = std::make_shared<VRChatRootCmd>(this);

	myCommands.push_back(commandRoot);

	myOSCRx.SetListener(&myOSCListener);
	myOSCRx.Open("127.0.0.1");
	myOSCTransmitter.Open("127.0.0.1");
}

VRChat::~VRChat()
{
	myOSCRx.Close();
	sock::Cleanup();
	m_running = false;
}

void VRChat::HandleEvent()
{
}

void VRChat::Update()
{

}

void VRChat::Draw()
{
	ImGui::Begin("Commands");


	if (ImGui::Button("Add Command"))
	{
		myCommands.push_back(std::make_shared<MessageCommand>(this));
	}

	ImGui::Indent();


	if(ImGui::TreeNodeEx("Commands"))
	{
		for (int i = 0; i < myCommands.size(); i++)
		{
			myCommands[i]->Draw();
			auto contextValue = myCommands[i]->DrawContextMenu(myCommands[i]->GetCommandName());

			switch(contextValue)
			{
			case ContextMenuReturn::Delete:
				myCommands.erase(myCommands.begin() + i);
				i--;
				break;
			case ContextMenuReturn::Copy:
				break;
			case ContextMenuReturn::Count:
				break;
			default:;
			}
		}

		ImGui::TreePop();
	}


	ImGui::Unindent();

	ImGui::End();
}

void VRChat::HandlePRIVMSG(const Lucent::ChatMessage& priv, bool tesedhjkashjkld)
{
	// THIS IS NOT USED ANYMORE
	auto [first, second] = SplitCommand(priv.Message);

	if (first == "vrchat")
	{
		const auto [commandType, commandData] = SplitCommand(second);

		
		if(commandType == "float")
		{
			// DONE
		}
		else if(commandType == "int")
		{
			// DONE
		}
		else if(commandType == "move")
		{
			// DONE
		}
		else if(commandType == "jump")
		{
			// DONE
		}
		else if(commandType == "look")
		{
			// DONE
		}
		else if(commandType == "speak")
		{
			// DONE
		}
		else if(commandType == "page")
		{
			// DONE
		}
		else if(commandType == "help")
		{
			// Maybe Sometime


			/*if(commandData == "bool")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "bool")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "float")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "int")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "move")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "look")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "speak")
			{
				SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData.empty())
			{
				SendPRIVMSG(priv.Channel, "Usage: !vrchat help [TYPE], Available Types: parameter");
			}*/
		}
	}
}

inline char asciitolower(char in) {
	if(in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

std::string VRChat::GetFullParameterName(const std::string& aChannel, const std::string& aToggleName, OSCType aType)
{

	if (myAvatarData.Parameters.size() <= 0)
	{
		SendPRIVMSG(aChannel, "Try Turning off and on OSC inside vrchat.");
		return "";
	}


	for(int i = 0; i < myAvatarData.Parameters.size(); i++)
	{
		std::string lowerCaseParameterName;
		lowerCaseParameterName.resize(myAvatarData.Parameters[i].ParameterName.size());
		std::transform(myAvatarData.Parameters[i].ParameterName.begin(), myAvatarData.Parameters[i].ParameterName.end(), lowerCaseParameterName.begin(), asciitolower);


		if(lowerCaseParameterName == aToggleName && myAvatarData.Parameters[i].Input.Type == aType)
		{
			return myAvatarData.Parameters[i].Input.Address;
		}
	}
	return "INVALID";
}

osc::Transmitter& VRChat::GetTransmitter()
{
	return myOSCTransmitter;
}

osc::Receiver& VRChat::GetReceiver()
{
	return myOSCRx;
}

const OSCData& VRChat::GetAvatarData() const
{
	return myAvatarData;
}

bool VRChat::IsAppRunning(const Lucent::ChatMessage& priv)
{
	if(!IsProgramRunning(L"VRChat"))
	{
		std::cout << "Please start vrchat and then press any key" << std::endl;
		SendPRIVMSG(priv.Channel, "Doesn't look Like VRChat is running on host computer.");
		return false;
	}

	return true;
}

bool VRChat::FindFile(std::filesystem::path path, std::filesystem::path& foundLocation, const std::string& aFilename , std::vector<std::filesystem::path>& filenames)
{
	const std::filesystem::directory_iterator end{};
	for(std::filesystem::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if(!std::filesystem::is_regular_file(*iter))
		{ 
			if(FindFile(iter->path(),  foundLocation, aFilename, filenames))
			{
				foundLocation = iter->path();
			}
		}
		else
		{
			filenames.push_back(iter->path().string());
		}
	}

	for (int i = 0; i < filenames.size(); i++)
	{
		if (filenames[i].filename() == aFilename)
		{
			foundLocation = filenames[i];
			return true;
		}
	}
	
	return false;
}
