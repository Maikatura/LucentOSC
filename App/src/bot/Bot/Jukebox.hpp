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

	void HandleEvent(const sf::Event& event) override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const PRIVMSG& priv) override;


private:
	bool m_running = true;
	State m_state = State::DoingNothing;
	std::thread m_playThread;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};
