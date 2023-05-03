#include "Chattu.h"
#include "bot/Client.hpp"

Chattu::Chattu(Client& client) : Bot(client)
{
}

Chattu::~Chattu()
{
}

void Chattu::HandleEvent(const sf::Event& event)
{
}

void Chattu::Update(sf::Time dt)
{
}

void Chattu::Draw(sf::RenderTarget& target)
{
}

void Chattu::HandlePRIVMSG(const PRIVMSG& priv)
{
	const auto [first, second] = SplitCommand(priv.message);

	if(first == "!chattu")
	{
		SendPRIVMSG(priv.Channel, "Hi, I am Chattu the bot inside this account!");
	}

	if(first == "!help")
	{
		SendPRIVMSG(priv.Channel, "Find me at https://github.com/maikatura/VRCBotTV");
	}
}
