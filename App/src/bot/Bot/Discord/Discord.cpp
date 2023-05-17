#include "Discord.h"

#include <fstream>

#include "imgui.h"
#include <json/json.hpp>

#include "bot/Client.hpp"
#include "bot/FileChecker.hpp"
#include "Fun/Pet.h"
#include "misc/print.h"
#include "Music/Join.h"
#include "Utility/Ping.h"
#include "VRChat/VRChatCmd.h"
#include <iomanip>


Discord::Discord(Lucent::TwitchApi& client) : Bot(client)
{
	myConnection = ConnectionStatus::Disconnected;
	//Start();
}

Discord::~Discord()
{
	Stop();
}

void Discord::HandleEvent()
{
}

void Discord::Update()
{
}

void Discord::Draw()
{
	ImGui::Begin("Discord");


	if (ImGui::Button("Start"))
	{
		Start();
	}
	ImGui::SameLine();
	if(ImGui::Button("Stop"))
	{
		Stop();
		
	}

	ImGui::Text("Status: ");
	ImGui::SameLine();
	switch(myConnection)
	{
		case ConnectionStatus::Connected:
			ImGui::Text("Connected");
			break;
		case ConnectionStatus::Disconnected:
			ImGui::Text("Disconnected");
			break;
		case ConnectionStatus::Count:
			ImGui::Text("?????");
			break;
	}

	ImGui::End();
}

void Discord::HandlePRIVMSG(const Lucent::ChatMessage& priv)
{
}

void Discord::Start()
{
	//if(myConnection == ConnectionStatus::Connected)
	//{
	//	return;
	//}


	//FileChecker::LoadFile<DiscordSettings>("data/user/discord.json", mySettings);

	//if(mySettings.Token.empty())
	//{
	//	return;
	//}


	//myCluster = new dpp::cluster(mySettings.Token, dpp::i_default_intents | dpp::i_message_content);
	//myCluster->on_log(dpp::utility::cout_logger());

	//myCommandHandler = new dpp::commandhandler(myCluster);
	//myCommandHandler->add_prefix(mySettings.Prefix).add_prefix("/");


	//myCluster->on_ready([&](const dpp::ready_t& event)
	//{
	///*	myDiscordCommands.push_back(std::make_shared<Ping>(*myCommandHandler, myCluster));
	//	myDiscordCommands.push_back(std::make_shared<Pet>(*myCommandHandler, myCluster));
	//	myDiscordCommands.push_back(std::make_shared<Join>(*myCommandHandler, myCluster));
	//	myDiscordCommands.push_back(std::make_shared<VRChatCmd>(*myCommandHandler, myCluster));*/

	//	///* NOTE: We must call this to ensure slash commands are registered.
	//	// * This does a bulk register, which will replace other commands
	//	// * that are registered already!
	//	// */
	//	//myCommandHandler->register_commands();


	//	if(dpp::run_once<struct register_bot_commands>())
	//	{
	//		myDiscordCommands.push_back(std::make_shared<Ping>(*myCommandHandler, myCluster));
	//		myDiscordCommands.push_back(std::make_shared<Pet>(*myCommandHandler, myCluster));
	//		myDiscordCommands.push_back(std::make_shared<Join>(*myCommandHandler, myCluster));
	//		myDiscordCommands.push_back(std::make_shared<VRChatCmd>(*myCommandHandler, myCluster));
	//	}
	//});

	//
	//myCluster->on_slashcommand([&](const dpp::slashcommand_t& event)
	//{

	//	for (int i = 0; i < myDiscordCommands.size(); i++)
	//	{
	//		if (event.command.get_command_name() == myDiscordCommands[i]->GetName())
	//		{
	//			myDiscordCommands[i]->Run(event);
	//			return;
	//		}
	//	}
	//});


	///* Use the on_message_create event to look for commands */
	//myCluster->on_message_create([&](const dpp::message_create_t& event)
	//{
	//	/*std::stringstream ss(event.msg.content);
	//	std::string command;
	//	ss >> command;

	//	for (int i = 0; i < myDiscordCommands.size(); i++)
	//	{
	//		if (myDiscordCommands[i]->GetName() == command)
	//		{
	//		}
	//	}*/
	//});

	//myCluster->on_ready([&](const dpp::ready_t& event)
	//{
	//	myConnection = ConnectionStatus::Connected;
	//});

	//myCluster->start(dpp::st_return);
}

void Discord::Stop()
{
	if(myConnection == ConnectionStatus::Disconnected)
	{
		return;
	}

	/*if (myCluster)
	{
		myCluster->shutdown();
		delete myCluster;
	}*/
	myConnection = ConnectionStatus::Disconnected;
}

bool Discord::IsAppRunning(const std::wstring& aApplication)
{
	if(!IsProgramRunning(aApplication.c_str()))
	{
		return false;
	}

	return true;
}
