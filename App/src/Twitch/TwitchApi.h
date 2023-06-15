#pragma once
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <WinSock2.h>



namespace Lucent
{
	enum class SubcriberTier
	{
		None,
		Tier1,
		Tier2,
		Tier3,
		Count
	};

	struct ChatMessage
	{
		std::string Username;
		std::string Message;
		std::string Nickname; // display name
		std::string Channel;
		bool IsSub = false;
		bool IsModerator = false;
		bool IsBroadcaster = false;
		bool IsVIP = false;
		bool IsTurbo = false;
		bool IsFounder = false;
		bool IsFirstMessage = false;
		int SubMonths = 0;
		SubcriberTier SubTier = SubcriberTier::None;
		unsigned int Color = 0;
	};

	class TwitchApi
	{
	public:
		bool Connect(const std::string& aOAuth, const std::string& aNickname);
		void Disconnect();

		void Join(const std::string& aChannel);
		void Join(std::vector<std::string> aJoinList);

		void Part(const std::string& aChannel);

		void SendChatMessage(const std::string& aChannel, const std::string& aMessage);

		void AddAdmin(const std::string& aChannel);
		void RemoveAdmin(const std::string& aChannel);
		bool IsAdmin(const std::string& aChannel); // TODO : Fix so that you can add and delete admins

		ChatMessage PopMessage();
		bool IsMessageQueueEmpty() const;

		std::vector<std::string>& GetJoinedChannels();

		const std::string& GetAccountName();

		ChatMessage ParseMessage(const std::string& aMessage);

	private:

		void Send(const std::string& aCmd);

		void NetworkLoop();

		SOCKET mySocketDescriptor {};


		std::string myAccountName;
		std::atomic_bool myNetworkIsWalking = false;
		std::thread myNetworkThread;
		std::queue<std::string> myChatMessages;
		std::vector<std::string> myJoinedChannels;
	};
}
