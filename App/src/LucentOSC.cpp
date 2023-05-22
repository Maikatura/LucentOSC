#include <fstream>
#include <iostream>
#include <thread>
#include "bot/Application.hpp"
#include <Twitch/TwitchApi.h>
#include <json/json.hpp>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

#if _DEBUG
#pragma warning( push )
#pragma warning( disable : 4996 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
#endif


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

#if _DEBUG
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
#endif

	return 0;
}

