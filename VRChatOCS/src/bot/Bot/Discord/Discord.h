#pragma once
#include "bot/Bot.hpp"
#include <dpp/dpp.h>


class Discord : public Bot
{

	enum class ConnectionStatus
	{
		Connected,
		Disconnected,
		Count
	};

public:
	explicit Discord(Client& client);
	~Discord();

	void HandleEvent(const sf::Event& event) override;
	void Update(sf::Time dt) override;
	void Draw(sf::RenderTarget& target) override;

	void HandlePRIVMSG(const PRIVMSG& priv) override;

private:

	void Start();
	void Stop();
	bool IsAppRunning(const std::wstring& aApplication);

	std::string myBotToken;
	dpp::cluster* myCluster;
	ConnectionStatus myConnection;
};
