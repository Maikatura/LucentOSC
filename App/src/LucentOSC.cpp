#include <fstream>
#include <iostream>
#include <thread>
#include "bot/Application.hpp"
#include <Twitch/TwitchApi.h>
#include <json/json.hpp>

int main()
{
	std::vector<std::string> myJoinChannel;
	std::vector<std::string> admins;

	std::ifstream ifsTwitch("data/user/twitch.json");
	nlohmann::json twitch = nlohmann::json::parse(ifsTwitch);


	for(auto& admin : twitch["admins"])
	{
		admins.push_back(admin.get<std::string>());
	}

	for(auto& channel : twitch["channels"])
	{
		myJoinChannel.push_back(channel.get<std::string>());
	}

	std::string oauth = twitch["oauth"].get<std::string>();
	std::string username = twitch["username"].get<std::string>();

	Lucent::TwitchApi client;
	if(client.Connect(oauth, username))
	{
		client.Join(myJoinChannel);
		Application app(client);
		app.Run();
	}
	else
	{
		std::cout << "Press Enter to exit... ";
		std::cin.get();
	}

	client.Disconnect();

	return 0;
}

