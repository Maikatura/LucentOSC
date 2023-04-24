#include "misc/print.h"
#include "osclib/osc.h"
#include "osclib/sock.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <thread>
#include <Windows.h>

#include "ConnectionFactory.h"
#include "bot/Client.hpp"
#include "bot/Application.hpp"
#include "misc/TimerManager.h"


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

	std::ifstream ifsAdmins("data/user/admins.txt");
	std::string strAdmins;
	while(std::getline(ifsAdmins, strAdmins))
	{
		admins.push_back(strAdmins);
	}

	std::ifstream ifs("data/user/channels.txt");
	std::string str;
	while(std::getline(ifs, str))
	{
		myJoinChannel.push_back(str);
	}

	std::string oauth;
	std::string username;
	if(std::ifstream ifs("data/user/login.txt"); !ifs || !std::getline(ifs, oauth) || !std::getline(ifs, username))
	{
		std::cout << "[Client] Failed to load config file!\n";
		return false;
	}

	Client client;
	//if(client.Connect(oauth, username, myJoinChannel, admins))
	//{
		Application app(client);
		app.run();
	//}
	/*else
	{
		std::cout << "Press Enter to exit... ";
		std::cin.get();
	}*/

	//client.disconnect();
}