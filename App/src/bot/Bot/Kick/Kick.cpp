#include "Kick.h"
#include "Downloader.h"

Kick::Kick(Lucent::TwitchApi& client) : Bot(client)
{
	myDownloadTimer = 5.0f;
	myDownloadCurrentTime = 0.0f;
}

Kick::~Kick()
{
}

void Kick::HandleEvent()
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

void Kick::HandlePRIVMSG(const Lucent::ChatMessage& priv)
{
}

void Kick::HandleKickParsing()
{
	auto test = Downloader::Request("maikatura");
	std::cout << test << std::endl;
}
