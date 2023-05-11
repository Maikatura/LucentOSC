#pragma once

#include "Bot/Bot.hpp"


class Kick : public Bot
{
public:
	explicit Kick(Client& client);
	~Kick();

	void HandleEvent(const sf::Event& event) override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const PRIVMSG& priv) override;

	void HandleKickParsing();

private:
	float myDownloadTimer;
	float myDownloadCurrentTime;
	
};
