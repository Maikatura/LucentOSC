#pragma once

#include "Bot.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Application
{
public:
	Application();
	Application(Client& client);
	Application(const Application& aApplication);

	bool run();
	bool IsRunning();
	void Stop();




private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void handlePRIVMSG(const PRIVMSG& priv);

private:
	Client& m_client;
	bool myIsRunning = true;
	std::vector<Bot::Ptr> m_bots;
};
