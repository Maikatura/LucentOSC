#include "VRChat.h"
#include <iostream>
#include "bot/Client.hpp"
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

VRChat::VRChat(Client& client) : Bot(client)
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
					sendPRIVMSG("#maikatura", "Changed into '" + myAvatarData.AvatarName + "'");
				}
				else
				{
					sendPRIVMSG("#maikatura", "Changed avatar from '"+ oldAvatarName+ "' to '" + myAvatarData.AvatarName + "'");
				}
			}
		}

		print_osc_value(addr, value);
	};


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

void VRChat::handleEvent(const sf::Event& event)
{
}

void VRChat::update(sf::Time dt)
{
}

void VRChat::draw(sf::RenderTarget& target)
{
}



void VRChat::handlePRIVMSG(const PRIVMSG& priv)
{
	const auto [first, second] = splitCommand(priv.message);

	if (first == "!vrchat")
	{
		const auto [commandType, commandData] = splitCommand(second);

		if (commandType == "bool")
		{
			if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = splitCommand(commandData);

			std::string parameterString = GetFullParameterName(priv.Channel, toggleName, OSCType::Bool);

			if(parameterString == "INVALID")
			{
				sendPRIVMSG(priv.Channel, "That Parameter is not a bool");
			}
			else if (!parameterString.empty())
			{
				if(toggleValue == "0" || toggleValue == "1" || toggleValue == "true" || toggleValue == "false")
				{
					if(toggleValue == "0" || toggleValue == "false")
					{
						myOSCTransmitter.SendBool(parameterString, false);
					}
					else
					{
						myOSCTransmitter.SendBool(parameterString, true);
					}
				}
				else
				{
					sendPRIVMSG(priv.Channel, "Something is wrong, The valid values is: 0, false, 1 and true");
				}
			}
		}
		else if(commandType == "float")
		{
			if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = splitCommand(commandData);

			if (toggleValue.empty() || ContainsOlyNumber(toggleValue) || AllSameChars(toggleValue, '.'))
			{
				sendPRIVMSG(priv.Channel, "Value is Null or Invalid Input.");
				return;
			}

			float value = std::stof(toggleValue);

			std::string parameterString = GetFullParameterName(priv.Channel, toggleName, OSCType::Float);
			if(parameterString == "INVALID")
			{
				sendPRIVMSG(priv.Channel, "That Parameter is not a float");
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

			sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
		}
		else if(commandType == "move")
		{
			if(!IsAppRunning(priv)) return;

			const auto [toggleName, toggleValue] = splitCommand(commandData);

			if (toggleName.empty())
			{
				sendPRIVMSG(priv.Channel, "Valid move types: forward, backward, left and right");
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

			const auto [toggleName, toggleValue] = splitCommand(commandData);

			if(toggleName.empty())
			{
				sendPRIVMSG(priv.Channel, "Valid move types: forward, backward, left and right");
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

			sendPRIVMSG(priv.Channel, pageParameters);
		}
		else if(commandType == "speak")
		{
			if(!IsAppRunning(priv)) return;

			if (isAdmin(priv.username))
			{
				myOSCTransmitter.SendChatboxMessage("/chatbox/input", commandData);
			}
			else
			{
				sendPRIVMSG(priv.Channel, "You can't use that command");
			}
		}
		else if(commandType == "help")
		{
			if(commandData == "bool")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "bool")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "float")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "int")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "move")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "look")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData == "speak")
			{
				sendPRIVMSG(priv.Channel, "Right now you can't find any commands since this is currently in DEV.");
			}
			else if(commandData.empty())
			{
				sendPRIVMSG(priv.Channel, "Usage: !vrchat help [TYPE], Available Types: parameter");
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
		sendPRIVMSG(aChannel, "Try Turning off and on OSC inside vrchat.");
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

bool VRChat::IsAppRunning(const PRIVMSG& priv)
{
	if(!IsProgramRunning(L"VRChat"))
	{
		std::cout << "Please start vrchat and then press any key" << std::endl;
		sendPRIVMSG(priv.Channel, "Doesn't look Like VRChat is running on host computer.");
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
