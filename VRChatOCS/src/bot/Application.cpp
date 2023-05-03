#include "Application.hpp"
#include "Client.hpp"
#include "Utility.hpp"

// bots
#include "Bot/Jukebox.hpp"
#include <Windows.h>
#include <dwmapi.h>

#include "imgui-SFML.h"
#include "Bot/VRChat/VRChat.h"
#include "Bot/Chattu.h"

#include "misc/TimerManager.h"

#include "imgui.h"
#include "imgui-SFML.h" 
#include "Bot/Discordu.h"

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

	if(HWND taskbar = FindWindow(L"Shell_TrayWnd", nullptr))
	{
		RECT rect;
		GetWindowRect(taskbar, &rect);

		const int width = rect.right - rect.left;
		const int height = rect.bottom - rect.top;

		if(width > height)
		{
			mode.height -= height;

			if(rect.top == 0)
			{
				windowPos.y = rect.bottom;
			}
		}
		else
		{
			mode.width -= width;

			if(rect.left == 0)
			{
				windowPos.x = rect.right;
			}
		}
	}


	// Should Remove this
	mode.width = 1280;
	mode.height = 720;

	myWindow.create(mode, "VRCBotTV", sf::Style::Default);
	//myWindow.setPosition(windowPos);
	myWindow.setFramerateLimit(60);
	ImGui::SFML::Init(myWindow);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	HWND hWnd = myWindow.getSystemHandle();

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
	m_bots.emplace_back(std::make_unique<Discord>(m_client));
}

Application::Application(const Application& aApplication) : Application(aApplication.m_client)
{
}

bool Application::Run()
{
	const sf::Time timePerFrame = sf::seconds(1.f / 60);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate;

	while (myIsRunning)
	{
		sf::Event event;
		while(myWindow.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(myWindow, event);
			if(event.type == sf::Event::Closed)
			{
				
				myWindow.close();
				ImGui::SFML::Shutdown(); // will shutdown all windows
				return 0; // return here so that we don't call Update/Render
			}
		}


		const sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		ImGui::SFML::Update(myWindow, dt);

		if (timeSinceLastUpdate >= timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			ProcessInput();
			Update(timePerFrame);
			TimerManager::Update();
		}

		Render();
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

void Application::ProcessInput()
{
	sf::Event event;

	while(myWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			myWindow.close();

		else if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Escape)
				myWindow.close();
		}

		for(auto& bot : m_bots)
			bot->HandleEvent(event);
	}

	while(!m_client.isMessageQueueEmpty())
	{
		PRIVMSG priv = m_client.popMessage();

		// HACK: auto join/part
		if(priv.message == "JOIN")
			priv.message = "!join";
		else if(priv.message == "PART")
			priv.message = "!part";

		if(!priv.message.empty() && priv.message[0] == '!')
		{
			HandlePRIVMSG(priv);

			for(auto& bot : m_bots)
				bot->HandlePRIVMSG(priv);
		}
	}
}

void Application::Update(sf::Time dt)
{
	for (auto& bot : m_bots)
		bot->Update(dt);
}

void Application::Render()
{
	myWindow.clear(sf::Color::Transparent);

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

	for(const auto& bot : m_bots)
		bot->Draw(myWindow);


	ImGui::SFML::SetCurrentWindow(myWindow);
	ImGui::Begin("Test");
	ImGui::Button("Example button");
	ImGui::End();


	ImGui::SFML::Render(myWindow);
	myWindow.display();
}

void Application::HandlePRIVMSG(const PRIVMSG& priv)
{
	if (!m_client.isAdmin(priv.username))
		return;

	auto [first, second] = SplitCommand(priv.message);
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
