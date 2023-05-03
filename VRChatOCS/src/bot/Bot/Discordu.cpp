#include "Discordu.h"

#include <fstream>

#include "imgui.h"
#include <json/json.hpp>

#include "bot/Client.hpp"
#include "misc/print.h"


Discord::Discord(Client& client) : Bot(client)
{
	myConnection = ConnectionStatus::Unknown;
	Start();
}

Discord::~Discord()
{
	Stop();
}

void Discord::HandleEvent(const sf::Event& event)
{
}

void Discord::Update(sf::Time dt)
{
}

void Discord::Draw(sf::RenderTarget& target)
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
		default: ;
	}

	ImGui::Text("This is the discord window");

	ImGui::Text("LOL");

	ImGui::End();
}

void Discord::HandlePRIVMSG(const PRIVMSG& priv)
{
}

void Discord::Start()
{
	if(myConnection == ConnectionStatus::Connected)
	{
		return;
	}

	std::ifstream ifsToken("data/user/discord.json");
	json parsedData = json::parse(ifsToken);

	if(!parsedData.contains("token"))
	{
		return;
	}

	myBotToken = parsedData["token"].get<std::string>();
	myCluster = new dpp::cluster(myBotToken, dpp::i_default_intents | dpp::i_message_content);


	myCluster->on_log(dpp::utility::cout_logger());

	myCluster->on_slashcommand([&](const dpp::slashcommand_t& event)
	{
		if(event.command.get_command_name() == "ping")
		{
			event.reply("Pong!");
		}

		if(event.command.get_command_name() == "vrchat")
		{
			if(IsAppRunning(L"VRChat"))
			{
				event.reply("VRChat is running!");
			}
			else
			{
				event.reply("VRChat is not running!");
			}
		}
	});

	myCluster->on_ready([&](const dpp::ready_t& event)
	{
		if(dpp::run_once<struct register_bot_commands>())
		{
			myCluster->global_command_create(
				dpp::slashcommand("ping", "Ping pong!", myCluster->me.id)
			);

			myCluster->global_command_create(
				dpp::slashcommand("vrchat", "Check if vrchat is running on host", myCluster->me.id)
			);
		}

		myConnection = ConnectionStatus::Connected;
	});

	myCluster->start(dpp::st_return);
}

void Discord::Stop()
{
	if(myConnection == ConnectionStatus::Disconnected)
	{
		return;
	}

	if (myCluster)
	{
		myCluster->shutdown();
		delete myCluster;
	}
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
