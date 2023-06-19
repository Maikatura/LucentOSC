#include "TwitchApi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <thread>

#include "bot/Utility.hpp"

#define DEFAULT_BUFLEN 512

bool Lucent::TwitchApi::Connect(const std::string& aOAuth, const std::string& aNickname)
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "Failed to initialize Winsock." << std::endl;
		return false;
	}

	const char* serverHostname = "irc.chat.twitch.tv";
	const char* serverPort = "6667";

	struct addrinfo hints, * result = nullptr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(serverHostname, serverPort, &hints, &result) != 0)
	{
		std::cerr << "Failed to resolve server hostname." << std::endl;
		WSACleanup();
		return false;
	}

	mySocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(mySocketDescriptor == INVALID_SOCKET)
	{
		std::cerr << "Failed to create socket." << std::endl;
		WSACleanup();
		return false;
	}


	if(connect(mySocketDescriptor, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if(errorCode != WSAEWOULDBLOCK)
		{
			std::cout << "Failed to connect to the server. Error code: " << errorCode << std::endl;
			closesocket(mySocketDescriptor);
			WSACleanup();
			return false;
		}
	}

	Send("PASS " + std::string(aOAuth));
	Send("NICK " + std::string(aNickname));

	Send("CAP REQ :twitch.tv/membership");
	Send("CAP REQ :twitch.tv/tags");
	Send("CAP REQ :twitch.tv/commands");




	myNetworkThread = std::thread([&]()
	{
		myNetworkIsWalking = true;
		NetworkLoop();

	});

	myAccountAuth = aOAuth;
	myAccountName = aNickname;


	return true;
}

void Lucent::TwitchApi::Disconnect()
{
	while(myNetworkIsWalking)
	{
		myNetworkIsWalking = false;
		myNetworkThread.join();
	}
}

void Lucent::TwitchApi::Join(const std::string& aChannel)
{
	myJoinedChannels.push_back(aChannel);
	Send("JOIN #" + aChannel);
}

void Lucent::TwitchApi::Join(std::vector<std::string> aJoinList)
{
	for(auto channel : aJoinList)
	{
		Join(channel);
	}
}

void Lucent::TwitchApi::Part(const std::string& aChannel)
{
	Send("PART #" + aChannel);
	for(int i = 0; i < myJoinedChannels.size(); i++)
	{
		if(myJoinedChannels[i] == aChannel)
		{
			myJoinedChannels.erase(myJoinedChannels.begin() + i);
			i--;
		}
	}
}

void Lucent::TwitchApi::SendChatMessage(const std::string& aChannel, const std::string& aMessage)
{
	Send("PRIVMSG #" + getSubstring(aChannel, "#") + " :" + aMessage);
}

void Lucent::TwitchApi::AddAdmin(const std::string& aChannel)
{
	aChannel;
}

void Lucent::TwitchApi::RemoveAdmin(const std::string& aChannel)
{
	aChannel;
}

bool Lucent::TwitchApi::IsAdmin(const std::string& aChannel)
{
	aChannel;
	return false;
}

Lucent::ChatMessage Lucent::TwitchApi::ParseMessage(const std::string& aMessage)
{
	ChatMessage chatMsg{};

	chatMsg.Username = getSubstring(aMessage, ":", "!");
	chatMsg.Message = getSubstring(aMessage, ".tmi.twitch.tv ", " ");

	if(chatMsg.Message == "PRIVMSG")
	{
		std::map<std::string, std::string> badgeInfoData;
		std::map<std::string, std::string> badgeData;

		auto message = getSubstring(aMessage, "@");
		auto split = Split(message, ";");

		for(size_t i = 0; i < split.size(); i++)
		{
			auto split2 = Split(split[i], "=");

			if(split2[0] == "badge-info")
			{
				for(size_t i = 1; i < split2.size(); i++)
				{
					auto dataValues = Split(split2[i], ",");
					for(size_t i = 0; i < dataValues.size(); i++)
					{
						auto dataAndValue = Split(dataValues[i], "/");
						badgeInfoData.insert({ dataAndValue[0],dataAndValue[1] });
					}
				}
			}
			else if(split2[0] == "badges")
			{
				for(size_t i = 1; i < split2.size(); i++)
				{
					auto dataValues = Split(split2[i], ",");
					for(size_t i = 0; i < dataValues.size(); i++)
					{
						auto dataAndValue = Split(dataValues[i], "/");
						badgeData.insert({ dataAndValue[0],dataAndValue[1] });
					}
				}
			}
		}

		// Data
		chatMsg.Channel = getSubstring(aMessage, "PRIVMSG ", " ");
		chatMsg.Message = getSubstring(aMessage, chatMsg.Channel + " :");
		chatMsg.IsFirstMessage = (getSubstring(aMessage, "first-msg=")[0] == '1') ? true : false;

		// Badges
		chatMsg.IsBroadcaster = (badgeData["broadcaster"] == "1") ? true : false;
		chatMsg.IsVIP = (badgeData["vip"] == "1") ? true : false;
		chatMsg.IsModerator = (getSubstring(aMessage, "mod=")[0] == '1') ? true : false;
		chatMsg.IsSub = (getSubstring(aMessage, "subscriber=", ";")[0] == '1') ? true : false;
		chatMsg.IsTurbo = (getSubstring(aMessage, "turbo=")[0] == '1') ? true : false;
		chatMsg.IsFounder = (getSubstring(aMessage, "founder=")[0] == '1') ? true : false;


		if (chatMsg.IsSub)
		{
			std::string subTier = badgeData["subscriber"];

			if(subTier.size() == 4)
			{
				if (subTier[0] == '3')
				{
					chatMsg.SubTier = SubcriberTier::Tier3;
				}
				else
				{
					chatMsg.SubTier = SubcriberTier::Tier2;
				}
			}
			else
			{
				chatMsg.SubTier = SubcriberTier::Tier1;
			}

			chatMsg.SubMonths = std::stoi(badgeInfoData["subscriber"]);
		}
	}

	chatMsg.Nickname = getSubstring(aMessage, "display-name=", ";");

	if(chatMsg.Nickname.empty())
		chatMsg.Nickname = chatMsg.Username;

	if(const std::string str = getSubstring(aMessage, "color=#", ";"); !str.empty())
		chatMsg.Color = std::stoi("0x" + str, 0, 16);

	return chatMsg;
}

Lucent::ChatMessage Lucent::TwitchApi::PopMessage()
{
	ChatMessage parsedMessage = ParseMessage(myChatMessages.front());
	myChatMessages.pop();
	return parsedMessage;

}

bool Lucent::TwitchApi::IsMessageQueueEmpty() const
{
	return myChatMessages.empty();
}

std::vector<std::string>& Lucent::TwitchApi::GetJoinedChannels()
{
	return myJoinedChannels;
}

std::string& Lucent::TwitchApi::GetAccountName()
{
	return myAccountName;
}

std::string& Lucent::TwitchApi::GetAccountAuth()
{
	return myAccountAuth;
}

void Lucent::TwitchApi::Send(const std::string& aCmd)
{
	std::string outMsg = aCmd + "\r\n";
	send(mySocketDescriptor, outMsg.c_str(), outMsg.length(), 0);
}

void Lucent::TwitchApi::NetworkLoop()
{
	const int bufferSize = 1024;
	char buffer[bufferSize];

	u_long mode = 1;
	if(ioctlsocket(mySocketDescriptor, FIONBIO, &mode) != 0)
	{
		std::cout << "Failed to set socket to non-blocking mode.\n";
		return;
	}

	while(myNetworkIsWalking)
	{
		int bytesReceived = recv(mySocketDescriptor, buffer, bufferSize - 1, 0);

		if(bytesReceived > 0)
		{
			buffer[bytesReceived] = '\0';

			std::string receivedData(buffer);

		#ifdef _DEBUG
			std::cout << receivedData << std::endl;
		#endif
			if(receivedData.substr(0, 4) == "PING")
			{
				std::string token = receivedData.substr(5);
				Send("PONG :" + token);
			}

			myChatMessages.push(receivedData);
		}
		else if(bytesReceived == 0)
		{
			// Connection closed by the remote peer
			std::cout << "Connection closed by the remote peer.\n";
			break;
		}
		else
		{
			int errorCode = WSAGetLastError();
			if(errorCode == WSAEWOULDBLOCK)
			{
				continue;
			}
			else if(errorCode == WSAECONNRESET)
			{
				// Connection reset by the remote host
				std::cout << "Connection reset by the remote host.\n";
				break;
			}
			else
			{
				// Other error occurred
				std::cout << "Error in recv. Error code: " << errorCode << std::endl;
				break;
			}
		}
	}

	closesocket(mySocketDescriptor);
	WSACleanup();
}
