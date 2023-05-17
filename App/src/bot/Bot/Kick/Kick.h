#pragma once

#include "Bot/Bot.hpp"


class Kick : public Bot
{
public:
	explicit Kick(Lucent::TwitchApi& client);
	~Kick();

	void HandleEvent() override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const Lucent::ChatMessage& priv) override;

	void HandleKickParsing();

private:
	float myDownloadTimer;
	float myDownloadCurrentTime;
	
};
