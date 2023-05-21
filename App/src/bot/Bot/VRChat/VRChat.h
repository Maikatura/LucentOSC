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
	explicit VRChat(Lucent::TwitchApi& client);
	~VRChat();

	void HandleEvent() override;
	void Update() override;
	void Draw() override;

	void HandlePRIVMSG(const Lucent::ChatMessage& priv, bool aIgnoreEnabledCheck = false) override;

	bool IsAppRunning(const Lucent::ChatMessage& priv);

	bool FindFile(std::filesystem::path path, std::filesystem::path& foundLocation, const std::string& aFilename, std::vector<std::filesystem::path>& filenames = std::vector<std::filesystem::path>());
	std::string GetFullParameterName(const std::string& aChannel, const std::string& aToggleName, OSCType aType);

	osc::Transmitter& GetTransmitter();
	osc::Receiver& GetReceiver();

private:


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
