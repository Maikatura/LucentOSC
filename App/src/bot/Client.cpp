#include "Client.hpp"
#include "Utility.hpp"

#include <SFML/Network/IpAddress.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

Client::Client()
{
}

bool Client::Connect(const std::string& aAuth, const std::string& aUsername, const std::vector<std::string> aChannels, const std::vector<std::string> someAdmins)
{
	if(m_socket.connect("irc.chat.twitch.tv", 6667) != sf::Socket::Done)
	{
		std::cout << "[Client] Failed to Connect to server!\n";
		return false;
	}

	m_channels.push_back(aChannels[0]);

	send("PASS " + aAuth + "\r\n");
	send("NICK " + aUsername + "\r\n");

	for (int i = 0;  i < aChannels.size(); i++)
	{
		std::string channel = "#" + aChannels[i];
		send("JOIN " + channel + "\r\n");
	}

	myAdmins = someAdmins;

	send("CAP REQ :twitch.tv/membership\r\n");
	send("CAP REQ :twitch.tv/tags\r\n");
	send("CAP REQ :twitch.tv/commands\r\n");

	m_thread = std::thread(&Client::receive, this);

	return true;
}

bool Client::Connect(const std::string& filename)
{
	std::string pass;
	std::string nick;

	if (std::ifstream ifs(filename); !ifs || !std::getline(ifs, pass) || !std::getline(ifs, nick) || !std::getline(ifs, m_channels[0]))
	{
		std::cout << "[Client] Failed to load config file!\n";
		return false;
	}

	if (m_socket.connect("irc.chat.twitch.tv", 6667) != sf::Socket::Done)
	{
		std::cout << "[Client] Failed to Connect to server!\n";
		return false;
	}

	send("PASS " + pass + "\r\n");
	send("NICK " + nick + "\r\n");
	send("JOIN " + m_channels[0] + "\r\n");

	send("CAP REQ :twitch.tv/membership\r\n");
	send("CAP REQ :twitch.tv/tags\r\n");
	send("CAP REQ :twitch.tv/commands\r\n");

	m_thread = std::thread(&Client::receive, this);

	return true;
}

void Client::disconnect()
{
	m_socket.disconnect();
	m_thread.join();
}

bool Client::isAdmin(const std::string& username) const
{
	for (int i = 0; i < myAdmins.size(); i++)
	{
		if (username == myAdmins[i])
		{
			return true;
		}
	}



	return false;
}

bool Client::isMessageQueueEmpty() const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	return m_messageQueue.empty();
}

PRIVMSG Client::popMessage()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	PRIVMSG priv = m_messageQueue.front();
	m_messageQueue.pop();

	return priv;
}

void Client::sendPRIVMSG(const std::string& msg, const std::string& channel)
{
	if (channel.empty())
	{
		send("PRIVMSG " + m_channels[0] + " :" + msg + "\r\n");
	}
	else
	{
		send("PRIVMSG " + channel + " :" + msg + "\r\n");
	}
}

void Client::AddAdmin(const std::string& aUsername)
{
	bool found = false;
	for (int i = 0; i < myAdmins.size(); i++)
	{
		if (aUsername == myAdmins[i])
		{
			found = true;
		}
	}

	if (!found)
	{
		myAdmins.push_back(aUsername);
	}

	SaveAdmins();
}

void Client::RemoveAdmin(const std::string& aUsername)
{
	for(int i = 0; i < myAdmins.size(); i++)
	{
		if(aUsername == myAdmins[i])
		{
			myAdmins.erase(myAdmins.begin() + i);
			i--;
		}
	}

	SaveAdmins();
}

PRIVMSG Client::parsePRIVMSG(const std::string& msg)
{
	PRIVMSG priv;

	priv.username = getSubstring(msg, ":", "!");
	priv.message = getSubstring(msg, ".tmi.twitch.tv ", " ");

	if (priv.message == "PRIVMSG")
	{
		priv.Channel = getSubstring(msg, "PRIVMSG ", " ");
		priv.message = getSubstring(msg, priv.Channel + " :");
	}


	priv.nickname = getSubstring(msg, "display-name=", ";");

	if (priv.nickname.empty())
		priv.nickname = priv.username;

	if (const std::string str = getSubstring(msg, "color=#", ";"); !str.empty())
		priv.color = std::stoi("0x" + str, 0, 16);

	return priv;
}

bool Client::send(const std::string& data)
{
	const std::string utf8 = ansiToUtf8(data);

	if (m_socket.send(utf8.data(), utf8.size()) != sf::Socket::Done)
	{
		std::cout << "[Client] Failed to send data!\n";
		return false;
	}

	#ifdef _DEBUG
	if (!startsWith(data, "PASS"))
		std::cout << "> " << data;
	#endif

	return true;
}

void Client::receive()
{
	char data[512];
	std::size_t received;

	while (m_socket.receive(data, sizeof(data), received) == sf::Socket::Done)
	{
		const std::string msg(data, received);

		#ifdef _DEBUG
		std::cout << "< " << msg;
		#endif

		if (msg == "PING :tmi.twitch.tv\r\n")
			send("PONG :tmi.twitch.tv\r\n");

		else
		{
			std::istringstream iss(utf8ToAnsi(msg));
			std::string str;

			while (std::getline(iss, str))
			{
				if (str.find("PRIVMSG") != std::string::npos ||
					str.find("JOIN") != std::string::npos ||
					str.find("PART") != std::string::npos)
				{
					if (str.back() == '\r')
						str.pop_back();

					std::lock_guard<std::mutex> lock(m_mutex);

					m_messageQueue.emplace(parsePRIVMSG(str));

					#ifndef _DEBUG
					const PRIVMSG& priv = m_messageQueue.back();
					std::cout << "[Twitch] " << priv.nickname << " " << priv.message << '\n';
					#endif
				}
			}
		}
	}
}

void Client::SaveAdmins()
{
	std::vector<std::string> savedUsernames;
	std::ofstream myfile;
	myfile.open("data/admins.txt");

	for (int i = 0; i < myAdmins.size(); i++)
	{
		for(int x = 0; x < savedUsernames.size(); x++)
		{
			if (savedUsernames[x] != myAdmins[i])
			{
				myfile << myAdmins[i] + "\n";
				savedUsernames.push_back(myAdmins[i]);
			}
		}
	}

	myfile.close();
}
