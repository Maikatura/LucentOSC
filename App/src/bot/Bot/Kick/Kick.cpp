#include "Kick.h"
#include "Downloader.h"

Kick::Kick(Client& client) : Bot(client)
{
	myDownloadTimer = 5.0f;
	myDownloadCurrentTime = 0.0f;
}

Kick::~Kick()
{
}

void Kick::HandleEvent(const sf::Event& event)
{
}

void Kick::Update()
{
	/*if (myDownloadCurrentTime <= 0.0f)
	{
		myDownloadCurrentTime = myDownloadTimer;
		HandleKickParsing();
	}
	else
	{
		myDownloadCurrentTime -= dt.asSeconds();
	}*/
}

void Kick::Draw()
{
}

void Kick::HandlePRIVMSG(const PRIVMSG& priv)
{
}

void Kick::HandleKickParsing()
{
	auto test = Downloader::Request("maikatura");
	std::cout << test << std::endl;
}
