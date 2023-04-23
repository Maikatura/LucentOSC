#pragma once

#include <SFML/Network/TcpSocket.hpp>


#include <thread>
#include <mutex>
#include <queue>

struct PRIVMSG
{
	std::string username;
	std::string message;
	std::string nickname; // display name
	std::string Channel;
	unsigned int color = 0;
};

class Client
{
public:

	Client();

	bool Connect(const std::string& aAuth, const std::string& aUsername, const std::vector<std::string> aChannels, const std::vector<std::string> someAdmins);
	bool Connect(const std::string& filename);
	void disconnect();

	bool isAdmin(const std::string& username) const;
	// TODO: bool isModerator(const std::string& username) const;

	bool isMessageQueueEmpty() const;
	PRIVMSG popMessage();

	void sendPRIVMSG(const std::string& msg, const std::string& channel = "");
	void AddAdmin(const std::string& aUsername);
	void RemoveAdmin(const std::string& aUsername);

	

private:
	PRIVMSG parsePRIVMSG(const std::string& msg);

	bool send(const std::string& data);
	void receive();

	void SaveAdmins();

private:
	sf::TcpSocket m_socket;
	std::vector<std::string> m_channels;
	std::vector<std::string> myAdmins;
	std::thread m_thread;
	mutable std::mutex m_mutex;
	std::queue<PRIVMSG> m_messageQueue;
};
