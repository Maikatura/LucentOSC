#include "Chattu.h"
#include "bot/Client.hpp"

Chattu::Chattu(Client& client) : Bot(client)
{
}

Chattu::~Chattu()
{
}

void Chattu::handleEvent(const sf::Event& event)
{
}

void Chattu::update(sf::Time dt)
{
}

void Chattu::draw(sf::RenderTarget& target)
{
}

void Chattu::handlePRIVMSG(const PRIVMSG& priv)
{
	const auto [first, second] = splitCommand(priv.message);

	if(first == "!chattu")
	{
		sendPRIVMSG(priv.Channel, "Hi, I am Chattu the bot inside this account!");
	}

	if(first == "!help")
	{
		sendPRIVMSG(priv.Channel, "Find me at https://github.com/maikatura/VRCBotTV");
	}
}
