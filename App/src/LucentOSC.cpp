#include "misc/print.h"
#include "osclib/osc.h"
#include "osclib/sock.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <thread>
#include <Windows.h>

#include "bot/Client.hpp"
#include "bot/Application.hpp"
#include "misc/TimerManager.h"
#include <dpp/dpp.h>
#include <tchar.h>

#include <Testing/TwitchApi.h>


std::string endpoint = "maikatura";

bool interrupted = false;


void onSIGINT(int)
{
	interrupted = true;
}

void msleep(int n)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(n));
}

int main()
{
	std::vector<std::string> myJoinChannel;
	std::vector<std::string> admins;

	std::ifstream ifsTwitch("data/user/twitch.json");
	json twitch = json::parse(ifsTwitch);


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
}

