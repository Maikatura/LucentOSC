#include "Application.hpp"
#include "Client.hpp"
#include "Utility.hpp"

// bots
#include "Bot/Jukebox.hpp"
#include <Windows.h>
#include <dwmapi.h>

#include "Bot/VRChat.h"
#include "Bot/Chattu.h"

#include "misc/TimerManager.h"

#pragma comment(lib, "dwmapi.lib")

Client nullClient;

Application::Application() : m_client(nullClient)
{

}

Application::Application(Client& client)
	: m_client(client)
{
	// TODO: https://stackoverflow.com/questions/9283430/how-to-set-icon-in-a-windows-application-using-visual-studio

	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	sf::Vector2i windowPos;

	if (HWND taskbar = FindWindow(L"Shell_TrayWnd", nullptr))
	{
		RECT rect;
		GetWindowRect(taskbar, &rect);

		const int width = rect.right - rect.left;
		const int height = rect.bottom - rect.top;

		if (width > height)
		{
			mode.height -= height;

			if (rect.top == 0)
				windowPos.y = rect.bottom;
		}

		else
		{
			mode.width -= width;

			if (rect.left == 0)
				windowPos.x = rect.right;
		}
	}

	/*m_window.create(mode, "MaruBot", sf::Style::None);
	m_window.setPosition(windowPos);
	m_window.setFramerateLimit(60);*/

	//HWND hWnd = m_window.getSystemHandle();

	//// always on top
	//// https://en.sfml-dev.org/forums/index.php?topic=2271.0
	//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	//// transparent
	//// https://gist.github.com/Alia5/5d8c48941d1f73c1ef14967a5ffe33d5
	//MARGINS margins = { -1 };
	//DwmExtendFrameIntoClientArea(hWnd, &margins);
	//SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	//// click-through
	//// https://stackoverflow.com/questions/31313624/click-through-transparent-window-no-dragging-allowed-c
	//SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);

	m_bots.emplace_back(std::make_unique<VRChat>(m_client));
	m_bots.emplace_back(std::make_unique<Chattu>(m_client));
}

Application::Application(const Application& aApplication) : Application(aApplication.m_client)
{
}

bool Application::run()
{
	const sf::Time timePerFrame = sf::seconds(1.f / 60);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate;

	while (myIsRunning)
	{
		const sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		if (timeSinceLastUpdate >= timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			processInput();
			update(timePerFrame);
			TimerManager::Update();
		}

		//render();
	}

	return false;
}

bool Application::IsRunning()
{
	return myIsRunning;
}

void Application::Stop()
{
	myIsRunning = false;
}

void Application::processInput()
{
	while (!m_client.isMessageQueueEmpty())
	{
		PRIVMSG priv = m_client.popMessage();

		// HACK: auto join/part
		if (priv.message == "JOIN")
			priv.message = "!join";
		else if (priv.message == "PART")
			priv.message = "!part";



		if (!priv.message.empty() && priv.message[0] == '!')
		{
			handlePRIVMSG(priv);

			for (auto& bot : m_bots)
				bot->handlePRIVMSG(priv);
		}
	}
}

void Application::update(sf::Time dt)
{
	for (auto& bot : m_bots)
		bot->update(dt);
}

void Application::render()
{
	
}

void Application::handlePRIVMSG(const PRIVMSG& priv)
{
	if (!m_client.isAdmin(priv.username))
		return;

	auto [first, second] = splitCommand(priv.message);
	toLower(second);




	if(first == "!quit")
	{
		myIsRunning = false;
	}
	else if (first == "!addbot" && !second.empty())
	{
		bool exists = false;

		for (const auto& bot : m_bots)
		{
			std::string name = typeid(*bot).name();
			toLower(name);

			if (endsWith(name, second))
			{
				exists = true;
				break;
			}
		}

		if (exists)
			m_client.sendPRIVMSG('@' + priv.username + ' ' + second + " is already running.");

		else
		{
			Bot::Ptr bot = nullptr;

			if (second == "vrchat")
				bot = std::make_unique<VRChat>(m_client);
			else if(second == "chattu")
				bot = std::make_unique<Chattu>(m_client);

			if (bot)
			{
				m_bots.emplace_back(std::move(bot));
				m_client.sendPRIVMSG('@' + priv.username + " Added bot: " + second);
			}

			else
				m_client.sendPRIVMSG('@' + priv.username + " Unrecognized bot name: " + second);
		}
	}
	else if (first == "!removebot" && !second.empty())
	{
		bool removed = false;

		for (auto it = m_bots.begin(); it != m_bots.end(); ++it)
		{
			std::string name = typeid(*(*it)).name();
			toLower(name);

			if (endsWith(name, second))
			{
				removed = true;
				m_bots.erase(it);
				break;
			}
		}

		if (removed)
			m_client.sendPRIVMSG('@' + priv.username + " Removed bot: " + second);
		else
			m_client.sendPRIVMSG('@' + priv.username + " Unrecognized bot name: " + second);
	}
	else if(first == "!addadmin" && !second.empty())
	{
		m_client.AddAdmin(second);
		m_client.sendPRIVMSG("Added '"  +second+ "' as admin", priv.Channel);
	}
	else if(first == "!removeadmin" && !second.empty())
	{
		m_client.RemoveAdmin(second);
		m_client.sendPRIVMSG("Removed '" + second + "' as admin", priv.Channel);
	}
}
