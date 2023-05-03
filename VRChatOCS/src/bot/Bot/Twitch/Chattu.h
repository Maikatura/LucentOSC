#pragma once

#include <filesystem>

#include "bot/Bot.hpp"

#include <queue>
#include <thread>
#include <mutex>

class Chattu : public Bot
{
public:
	enum class State
	{
		DoingNothing,
		PlayingDefaultSong,
		PlayingRequestedSong,
	};

public:
	explicit Chattu(Client& client);
	~Chattu();

	void HandleEvent(const sf::Event& event) override;
	void Update(sf::Time dt) override;
	void Draw(sf::RenderTarget& target) override;

	void HandlePRIVMSG(const PRIVMSG& priv) override;


private:
	bool m_running = true;
	State m_state = State::DoingNothing;
	std::thread m_playThread;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};
