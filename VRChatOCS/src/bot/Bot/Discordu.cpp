#include "Discordu.h"

#include <fstream>


Discord::Discord(Client& client) : Bot(client)
{

	std::ifstream ifsToken("data/user/discord.txt");
	std::getline(ifsToken, myBotToken);
	ifsToken.close();

	myCluster = new dpp::cluster(myBotToken, dpp::i_default_intents | dpp::i_message_content);

	auto& bot = *myCluster;

	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand([](const dpp::slashcommand_t& event)
	{
		if(event.command.get_command_name() == "ping")
		{
			event.reply("Pong!");
		}
	});

	bot.on_ready([&bot](const dpp::ready_t& event)
	{
		if(dpp::run_once<struct register_bot_commands>())
		{
			bot.global_command_create(
				dpp::slashcommand("ping", "Ping pong!", bot.me.id)
			);
		}
	});

	bot.start(dpp::st_return);
}

Discord::~Discord()
{
	delete myCluster;
}

void Discord::handleEvent(const sf::Event& event)
{
}

void Discord::update(sf::Time dt)
{
}

void Discord::draw(sf::RenderTarget& target)
{
}

void Discord::handlePRIVMSG(const PRIVMSG& priv)
{
}
