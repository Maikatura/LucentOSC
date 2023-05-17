#pragma once
#include "bot/Bot.hpp"
#include <dpp/dpp.h>

#include "bot/DiscordSettings.h"


class Discord : public Bot
{

	enum class ConnectionStatus
	{
		Connected,
		Disconnected,
		Count
	};

public:
	explicit Discord(Lucent::TwitchApi& client);
	~Discord();

	void HandleEvent() override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const Lucent::ChatMessage& priv) override;

private:

	void Start();
	void Stop();
	bool IsAppRunning(const std::wstring& aApplication);

	dpp::cluster* myCluster;
	dpp::commandhandler* myCommandHandler;

	DiscordSettings mySettings;
	ConnectionStatus myConnection;
};
