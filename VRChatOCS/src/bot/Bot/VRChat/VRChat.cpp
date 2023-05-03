#include "VRChat.h"
#include <iostream>

#include "imgui.h"
#include "bot/Client.hpp"
#include "Commands/VRChatRootCmd.h"
#include "misc/print.h"
#include "misc/TimerManager.h"
#include "osclib/sock.h"

bool ContainsOlyNumber(std::string const& str) {
	return str.find_first_of("1234567890.") ==
		std::string::npos;
}

bool AllSameChars(std::string testStr, char aChar) {
	return testStr.find_first_not_of(aChar) == std::string::npos;
}

VRChat::VRChat(Client& client) : ::Bot(client)
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
		//print_hex_dump(ptr, len);
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

		print_osc_value(addr, value);
	};

	auto commandRoot = std::make_shared<VRChatRootCmd>(this, "vrchat");
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

void VRChat::HandleEvent(const sf::Event& event)
{
}

void VRChat::Update(sf::Time dt)
{
}

void VRChat::Draw(sf::RenderTarget& target)
{
	ImGui::Begin("Twitch");

	ImGui::End();
}

void VRChat::HandlePRIVMSG(const PRIVMSG& priv)
{
	auto [first, second] = SplitCommand(priv.message);
	first.erase(0, 1);

	for (int i = 0; i < myCommands.size(); i++)
	{
		if (myCommands[i]->IsCommand(first))
		{
			if (myCommands[i]->HandleCommand(myClient, priv, second))
			{
				return;
			}
		}
	}

	
	if (first == "!vrchat")
	{
		const auto [commandType, commandData] = SplitCommand(second);

		
		if(commandType == "float")
		{
			if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = SplitCommand(commandData);

			if (toggleValue.empty() || ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
			{
				SendPRIVMSG(priv.Channel, "Value is Null or Invalid Input.");
				return;
			}

			float value = std::stof(toggleValue);

			std::string parameterString = GetFullParameterName(priv.Channel, toggleName, OSCType::Float);
			if(parameterString == "INVALID")
			{
				SendPRIVMSG(priv.Channel, "That Parameter is not a float");
			}
			else if (!parameterString.empty())
			{
				if(value >= 0.0f && value <= 1.0f)
				{
					myOSCTransmitter.SendFloat(parameterString, value);
				}
			}
			
		}
		else if(commandType == "int")
		{
			if(!IsAppRunning(priv)) return;

			SendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
		}
		else if(commandType == "move")
		{
			if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = SplitCommand(commandData);

			if (toggleName.empty())
			{
				SendPRIVMSG(priv.Channel, "Valid move types: forward, backward, left and right");
				return;
			}



			if (toggleName == "forward")
			{
				myOSCTransmitter.SendInt("/input/MoveForward", 1);
				myOSCTransmitter.SendInt("/input/MoveBackward", 0);

				std::string value = toggleValue;
				if (toggleValue.empty() && ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
				{
					value = "1";
				}

				TimerManager::AddTimer([&]()
				{
					myOSCTransmitter.SendInt("/input/MoveForward", 0);
				}, std::stof(value), false);

			}
			else if(toggleName == "backward")
			{
				myOSCTransmitter.SendInt("/input/MoveBackward", 1);

				std::string value = toggleValue;
				if(toggleValue.empty() && ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
				{
					value = "1";
				}

				TimerManager::AddTimer([&]()
				{
					myOSCTransmitter.SendInt("/input/MoveBackward", 0);
				}, std::stof(value), false);
			}
			else if (toggleName == "left")
			{
				
				myOSCTransmitter.SendInt("/input/MoveLeft", 1);
			

				std::string value = toggleValue;
				if(toggleValue.empty() && ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
				{
					value = "1";
				}

				TimerManager::AddTimer([&]()
				{
					myOSCTransmitter.SendInt("/input/MoveLeft", 0);
				}, std::stof(value), false);
			}
			else if(toggleName == "right")
			{
				myOSCTransmitter.SendInt("/input/MoveRight", 1);

				std::string value = toggleValue;
				if(toggleValue.empty() && ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
				{
					value = "1";
				}

				TimerManager::AddTimer([&]()
				{
					myOSCTransmitter.SendInt("/input/MoveRight", 0);
				}, std::stof(value), false);
			}
			else if (toggleName == "stop")
			{
				myOSCTransmitter.SendInt("/input/MoveLeft", 0);
				myOSCTransmitter.SendInt("/input/MoveRight", 0);
				myOSCTransmitter.SendInt("/input/MoveForward", 0);
				myOSCTransmitter.SendInt("/input/MoveBackward", 0);
			}
		}
		else if(commandType == "jump")
		{
			if(!IsAppRunning(priv)) return;

			myOSCTransmitter.SendInt("/input/Jump", 1);


			TimerManager::AddTimer([&]()
			{
				myOSCTransmitter.SendInt("/input/Jump", 0);
			}, 0.1f, false);
		}
		else if(commandType == "look")
		{
			if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = SplitCommand(commandData);

			if(toggleName.empty())
			{
				SendPRIVMSG(priv.Channel, "Valid move types: forward, backward, left and right");
				return;
			}



			if(toggleName == "right")
			{
				myOSCTransmitter.SendInt("/input/LookRight", 1);

				std::string value = toggleValue;
				if(toggleValue.empty() && ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
				{
					value = "1";
				}

				TimerManager::AddTimer([&]()
				{
					myOSCTransmitter.SendInt("/input/LookRight", 0);
				}, std::stof(value), false);

			}
			else if(toggleName == "left")
			{
				myOSCTransmitter.SendInt("/input/LookLeft", 1);

				std::string value = toggleValue;
				if(toggleValue.empty() && ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
				{
					value = "1";
				}

				TimerManager::AddTimer([&]()
				{
					myOSCTransmitter.SendInt("/input/LookLeft", 0);
				}, std::stof(value), false);

			}
			else if(toggleName == "stop")
			{
				myOSCTransmitter.SendInt("/input/MoveLeft", 0);
				myOSCTransmitter.SendInt("/input/MoveRight", 0);
				myOSCTransmitter.SendInt("/input/MoveForward", 0);
				myOSCTransmitter.SendInt("/input/MoveBackward", 0);
			}
		}
		else if(commandType == "avatar")
		{
			/*if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = splitCommand(commandData);

			if(toggleName.empty())
			{
				sendPRIVMSG(priv.Channel, "Valid move types: forward, backward, left and right");
				return;
			}



			if(toggleName == "change")
			{
				myOSCTransmitter.SendString("/avatar/change", toggleValue);
			}*/
		}
		else if(commandType == "page")
		{
			if(!IsAppRunning(priv)) return;

			int page;
			if (commandData.empty())
			{
				page = 1;
			}
			else
			{
				page = std::stoi(commandData);
			}


			page = 5 * page - 1;

			if (page < 0)
			{
				page = 0;
			}

			std::string pageParameters = "Page ";
			pageParameters += commandData;
			pageParameters += ": ";

			for (int i = 0; i < 5; i++)
			{
				if (page + i <= myAvatarData.Parameters.size())
				{

					if (i == 4)
					{
						pageParameters += myAvatarData.Parameters[page + i].ParameterName;
					}
					else
					{
						pageParameters += myAvatarData.Parameters[page + i].ParameterName + ", ";
						
					}
				}
			}

			SendPRIVMSG(priv.Channel, pageParameters);
		}
		else if(commandType == "speak")
		{
			if(!IsAppRunning(priv)) return;

			if (IsAdmin(priv.username))
			{
				myOSCTransmitter.SendChatboxMessage("/chatbox/input", commandData);
			}
			else
			{
				SendPRIVMSG(priv.Channel, "You can't use that command");
			}
		}
		else if(commandType == "help")
		{
			if(commandData == "bool")
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
			}
		}
	}
}

char asciitolower(char in) {
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

bool VRChat::IsAppRunning(const PRIVMSG& priv)
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
