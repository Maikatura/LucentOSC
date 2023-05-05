#include "Application.hpp"
#include "Client.hpp"
#include "Utility.hpp"

// bots
#include "Bot/Jukebox.hpp"
#include <Windows.h>
#include <dwmapi.h>

#include "FileChecker.hpp"
#include "imgui-SFML.h"


#include "misc/TimerManager.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Bot/Kick/Kick.h"
#include "Bot/VRChat/VRChat.h"
#include "Bot/Twitch/Chattu.h"
#include "Bot/Discord/Discord.h"

#pragma comment(lib, "dwmapi.lib")

Client nullClient;

inline bool ExistsTest(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

Application::Application() : m_client(nullClient)
{

}

Application::Application(Client& client)
	: m_client(client)
{

	std::string settingsPath = "data/user/settings.json";

	mySettings = FileChecker::LoadFile<Settings>(settingsPath);

	if (!mySettings.Headless)
	{
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

		/*io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF("data/fonts/OpenSans-Light.ttf", 16);
		io.Fonts->AddFontFromFileTTF("data/fonts/OpenSans-Regular.ttf", 16);
		io.Fonts->AddFontFromFileTTF("data/fonts/OpenSans-Light.ttf", 32);
		io.Fonts->AddFontFromFileTTF("data/fonts/OpenSans-Regular.ttf", 11);
		io.Fonts->AddFontFromFileTTF("data/fonts/OpenSans-Bold.ttf", 11);
		io.Fonts->Build();*/

		ImGui::SFML::UpdateFontTexture();

		// Rounded Visual Studio style by RedNicStone from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 4.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 4.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 2.5f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 11.0f;
		style.ScrollbarRounding = 2.5f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 2.0f;
		style.TabRounding = 3.5f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5921568870544434f, 0.5921568870544434f, 0.5921568870544434f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	}
	


	m_bots.emplace_back(std::make_unique<VRChat>(m_client));
	m_bots.emplace_back(std::make_unique<Chattu>(m_client));
	m_bots.emplace_back(std::make_unique<Discord>(m_client));
	m_bots.emplace_back(std::make_unique<Kick>(m_client));
}

Application::Application(const Application& aApplication) : Application(aApplication.m_client)
{
}

bool Application::Run()
{
	const sf::Time timePerFrame = sf::seconds(1.f / 60.0f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate;

	while (myIsRunning)
	{
		if (!mySettings.Headless)
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
		}

		const sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		if(!mySettings.Headless)
		{
			ImGui::SFML::Update(myWindow, dt);
		}

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
		{
			myWindow.close();
		}
		else if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Escape)
			{
				myWindow.close();
			}
		}

		for(auto& bot : m_bots)
		{
			bot->HandleEvent(event);
		}
	}

	while(!m_client.isMessageQueueEmpty())
	{
		PRIVMSG priv = m_client.popMessage();

		// HACK: auto join/part
		if(priv.message == "JOIN")
		{
			priv.message = "!join";
		}
		else if(priv.message == "PART")
		{
			priv.message = "!part";
		}

		if(!priv.message.empty() && priv.message[0] == '!')
		{
			HandlePRIVMSG(priv);

			for(auto& bot : m_bots)
			{
				bot->HandlePRIVMSG(priv);
			}
		}
	}
}

void Application::Update(sf::Time dt)
{
	for (auto& bot : m_bots)
	{
		bot->Update(dt);
	}
}

void Application::Render()
{
	if (mySettings.Headless)
	{
		return;
	}

	myWindow.clear(sf::Color::Transparent);

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);
	ImGui::SFML::SetCurrentWindow(myWindow);

	for(const auto& bot : m_bots)
	{
		bot->Draw(myWindow);
	}


	ImGui::SFML::Render(myWindow);
	myWindow.display();
}

void Application::HandlePRIVMSG(const PRIVMSG& priv)
{
	if (!m_client.isAdmin(priv.username))
	{
		return;
	}

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
		{
			m_client.sendPRIVMSG('@' + priv.username + ' ' + second + " is already running.");
		}
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
			{
				m_client.sendPRIVMSG('@' + priv.username + " Unrecognized bot name: " + second);
			}
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
		{
			m_client.sendPRIVMSG('@' + priv.username + " Removed bot: " + second);
		}
		else
		{
			m_client.sendPRIVMSG('@' + priv.username + " Unrecognized bot name: " + second);
		}
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
