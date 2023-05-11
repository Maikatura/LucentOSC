#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

#include "Command.h"
#include "Bot/Discord/Base/CommandBase.h"

class Client;
struct PRIVMSG;

class Bot
{
public:
	using Ptr = std::unique_ptr<Bot>;

public:
	explicit Bot(Client& client);
	virtual ~Bot() = default;

	Bot(const Bot&) = delete;
	Bot& operator=(const Bot&) = delete;

	virtual void HandleEvent(const sf::Event& event) = 0;
	virtual void Update() {};
	virtual void Draw() = 0;

	virtual void HandlePRIVMSG(const PRIVMSG& priv) = 0;

protected:
	bool IsAdmin(const std::string& username) const;
	void SendPRIVMSG(const std::string& aChannel, const std::string& msg);

	std::vector<std::shared_ptr<Command>> myCommands;
	std::vector<std::shared_ptr<CommandBase>> myDiscordCommands;

	Client& myClient;
private:
};

std::pair<std::string, std::string> SplitCommand(const std::string& command);
