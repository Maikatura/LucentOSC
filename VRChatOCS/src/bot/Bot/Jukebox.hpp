#pragma once

#include "../Bot.hpp"

#include <queue>
#include <thread>
#include <mutex>

class Jukebox : public Bot
{
public:
	enum class State
	{
		DoingNothing,
		PlayingDefaultSong,
		PlayingRequestedSong,
	};

public:
	explicit Jukebox(Client& client);
	~Jukebox();

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time dt) override;
	void draw(sf::RenderTarget& target) override;

	void handlePRIVMSG(const PRIVMSG& priv) override;


private:
	bool m_running = true;
	State m_state = State::DoingNothing;
	std::thread m_playThread;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};
