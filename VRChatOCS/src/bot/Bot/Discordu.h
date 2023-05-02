#pragma once
#include "bot/Bot.hpp"
#include <dpp/dpp.h>


class Discord : public Bot
{
public:
	explicit Discord(Client& client);
	~Discord();

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time dt) override;
	void draw(sf::RenderTarget& target) override;

	void handlePRIVMSG(const PRIVMSG& priv) override;

private:
	std::string myBotToken;
	dpp::cluster* myCluster;
};
