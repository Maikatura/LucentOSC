#pragma once

#include <filesystem>

#include "Bot/Bot.hpp"

#include <queue>
#include <thread>
#include <mutex>

#include "osclib/osc.h"
#include "osclib/OSCData.h"

class VRChat : public Bot
{
public:
	enum class State
	{
		DoingNothing,
		PlayingDefaultSong,
		PlayingRequestedSong,
	};

public:
	explicit VRChat(Client& client);
	~VRChat();

	void HandleEvent(const sf::Event& event) override;
	void Update(sf::Time dt) override;
	void Draw(sf::RenderTarget& target) override;

	void HandlePRIVMSG(const PRIVMSG& priv) override;

	bool IsAppRunning(const PRIVMSG& priv);

	bool FindFile(std::filesystem::path path, std::filesystem::path& foundLocation, const std::string& aFilename, std::vector<std::filesystem::path>& filenames = std::vector<std::filesystem::path>());

private:

	std::string GetFullParameterName(const std::string& aChannel, const std::string& aToggleName, OSCType aType);

	bool m_running = true;
	State m_state = State::DoingNothing;
	std::thread m_playThread;
	std::mutex m_mutex;
	std::condition_variable m_condition;

	
	


	OSCData myAvatarData;
	osc::Listener myOSCListener;
	osc::Receiver myOSCRx;
	osc::Transmitter myOSCTransmitter;
};
