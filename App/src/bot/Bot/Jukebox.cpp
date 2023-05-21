#include "Jukebox.hpp"
#include "../Client.hpp"
#include "../Utility.hpp"

#include <Windows.h>

#include <iostream>
#include <filesystem>
#include <random>
#include <array>

namespace fs = std::filesystem;

Jukebox::Jukebox(Lucent::TwitchApi& client)
	: Bot(client)
{
	
}

Jukebox::~Jukebox()
{
	m_running = false;
}

void Jukebox::HandleEvent()
{
}

void Jukebox::Update()
{
}

void Jukebox::Draw()
{
}

void Jukebox::HandlePRIVMSG(const Lucent::ChatMessage& priv, bool aIgnoreEnabledCheck)
{
	const auto [first, second] = SplitCommand(priv.Message);


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
			SendPRIVMSG(priv.Channel, '@' + priv.Username + " Usage: !sr [a link or song title] (Supported sites: https://rg3.github.io/youtube-dl/supportedsites.html)");
	}

	else if (first == "!skip")
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_state == State::PlayingDefaultSong)
		{
			SendPRIVMSG(priv.Channel, '@' + priv.Username + " Skipped the current song.");
		}

		else
			SendPRIVMSG(priv.Channel, '@' + priv.Username + " Requested songs can't be skipped.");
	}

}
