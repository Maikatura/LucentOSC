#pragma once

#include "Bot.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Settings.hpp"

class Application
{
public:
	Application();
	Application(Client& client);
	Application(const Application& aApplication);

	bool Run();
	bool IsRunning();
	void Stop();


private:
	void ProcessInput();
	void Update(sf::Time dt);
	void Render();

	void HandlePRIVMSG(const PRIVMSG& priv);

private:
	Settings mySettings;
	Client& m_client;
	bool myIsRunning = true;
	sf::RenderWindow myWindow;
	std::vector<Bot::Ptr> m_bots;
};
