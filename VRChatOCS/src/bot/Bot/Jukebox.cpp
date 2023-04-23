#include "Jukebox.hpp"
#include "../Client.hpp"
#include "../Utility.hpp"

#include <Windows.h>

#include <iostream>
#include <filesystem>
#include <random>
#include <array>

namespace fs = std::filesystem;

Jukebox::Jukebox(Client& client)
	: Bot(client)
{
	
}

Jukebox::~Jukebox()
{
	m_running = false;
}

void Jukebox::handleEvent(const sf::Event& event)
{
}

void Jukebox::update(sf::Time dt)
{
}

void Jukebox::draw(sf::RenderTarget& target)
{
}

void Jukebox::handlePRIVMSG(const PRIVMSG& priv)
{
	const auto [first, second] = splitCommand(priv.message);


	if (first == "!songrequest" || first == "!sr")
	{
		if (!second.empty())
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);

			
			}

			m_condition.notify_all();
		}

		else
			sendPRIVMSG(priv.Channel, '@' + priv.username + " Usage: !sr [a link or song title] (Supported sites: https://rg3.github.io/youtube-dl/supportedsites.html)");
	}

	else if (first == "!skip")
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_state == State::PlayingDefaultSong)
		{
			sendPRIVMSG(priv.Channel, '@' + priv.username + " Skipped the current song.");
		}

		else
			sendPRIVMSG(priv.Channel, '@' + priv.username + " Requested songs can't be skipped.");
	}

}
