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
	explicit Chattu(Lucent::TwitchApi& client);
	~Chattu();

	void HandleEvent() override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const Lucent::ChatMessage& priv, bool aIgnoreEnabledCheck = false) override;


private:
	bool m_running = true;
	State m_state = State::DoingNothing;
	std::thread m_playThread;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};
