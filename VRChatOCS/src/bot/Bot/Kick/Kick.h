#pragma once

#include "Bot/Bot.hpp"


class Kick : public Bot
{
public:
	explicit Kick(Client& client);
	~Kick();

	void HandleEvent(const sf::Event& event) override;
	void Update(sf::Time dt) override;
	void Draw(sf::RenderTarget& target) override;

	void HandlePRIVMSG(const PRIVMSG& priv) override;

	void HandleKickParsing();

private:
	float myDownloadTimer;
	float myDownloadCurrentTime;
	
};
