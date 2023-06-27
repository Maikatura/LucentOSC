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
	nlohmann::json twitch;

	std::ifstream ifsTwitch("data/user/twitch.json");

	if (!ifsTwitch.good())
	{
		MessageBoxA(NULL, "Unable to open JSON file `data/user/twitch.json`.", "Error", MB_OK);
		return -1;
	}

	try
	{
		twitch = nlohmann::json::parse(ifsTwitch);
	}
	catch (const nlohmann::json::parse_error& e)
	{
		MessageBoxA(NULL, "Unable to parse JSON file `data/user/twitch.json`.", "Error", MB_OK);
		return -2;
	}

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

	Application app(client);

	if(client.Connect(oauth, username))
	{
		client.Join(myJoinChannel);
	}
	else
	{
		app.ShowConfig();
	}

	app.Run();

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

