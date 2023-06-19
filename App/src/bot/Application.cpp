#include "Application.hpp"
#include "Utility.hpp"

// bots
#include "Bot/Jukebox.hpp"
#include <Windows.h>
#include <dwmapi.h>

#include "FileChecker.hpp"



#include "misc/TimerManager.h"

#include "Bot/Kick/Kick.h"
#include "Bot/VRChat/VRChat.h"
#include "Bot/Twitch/Chattu.h"
#include "Bot/Discord/Discord.h"

#include <shellapi.h>

#include "Framework/DX11.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "misc/ConsoleLog.h"
#include <tray.hpp>

#include "imgui_internal.h"
#include "fonts/IconsForkAwesome.h"

#pragma comment(lib, "dwmapi.lib")

Lucent::TwitchApi nullClient;

inline bool ExistsTest(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

Application::Application() : myClient(nullClient)
{
	
}

Application::Application(Lucent::TwitchApi& client)
	: myClient(client)
{

	std::string settingsPath = "data/user/settings.json";

	FileChecker::LoadFile<Settings>(settingsPath, mySettings);



	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = Application::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"OSC";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"OSC",
		L"LucentOSC",
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		0,
		0,
		1280,
		720,
		nullptr, nullptr, nullptr,
		this
	);

	if (!DX11::Init(myWindowHandle, false))
	{
		std::cout << "Failed to create DX11 Stuff" << std::endl;
	}


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui_ImplWin32_Init(myWindowHandle);
	ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());

	io.Fonts->AddFontDefault();

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FK, 13.0f, &config, icon_ranges);

	myBots.emplace_back(std::make_unique<VRChat>(myClient));
	myBots.emplace_back(std::make_unique<Chattu>(myClient));
	//myBots.emplace_back(std::make_unique<Kick>(myClient));
	//myBots.emplace_back(std::make_unique<Discord>(myClient));

	SetupImGuiStyle();

	memset(&notifyData, 0, sizeof(NOTIFYICONDATA));
	notifyData.cbSize = sizeof(NOTIFYICONDATA);
	notifyData.hWnd = myWindowHandle;
	notifyData.uFlags = NIF_ICON | NIF_MESSAGE;
	notifyData.uCallbackMessage = WM_TRAY;
	//notifyData.hIcon = this->icon;

	if(Shell_NotifyIcon(NIM_ADD, &notifyData) == FALSE)
	{
		throw std::runtime_error("Failed to register tray icon");
	}
	trayList.insert({ myWindowHandle, *this });

	SetUpTrayMenu();

}

Application::Application(const Application& aApplication) : Application(aApplication.myClient)
{
}

bool Application::Run()
{
	// Main loop

	const int FPS = 60;
	const int FRAME_TIME = 1000 / FPS;

	myIsRunning = true;
	while(myIsRunning)
	{
		MSG msg = { 0 };
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{
				myIsRunning = false;
			}
		}

		if(!myIsRunning)
		{
			break;
		}


		Update();
		ProcessInput();


		if(!myIsMinimized)
		{
			if(myWantToResizeBuffers)
			{
				if(DX11::SwapChain)
				{
					DX11::Resize();
				}
				myWantToResizeBuffers = false;
			}

			auto start = std::chrono::high_resolution_clock::now();

			// Start the Dear ImGui frame
			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_NewFrame();
			ImGui::NewFrame();

			std::array<float, 4> clear_color[4] = { 0.0f, 0.0f, 0.0f ,1.0f };
			DX11::BeginFrame(clear_color[0]);

			{
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

				
				if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace Demo", nullptr, window_flags);
				ImGui::PopStyleVar();

				ImGui::PopStyleVar(2);

				
				ImGuiIO& io = ImGui::GetIO();
				if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("DX11AppDockspace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				
				RenderMenu();
				Render();

				ImGui::End();
			}


			// Rendering
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			ImGuiIO& io = ImGui::GetIO();
			if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			DX11::EndFrame();

			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			if(elapsed_time < FRAME_TIME)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_TIME - elapsed_time));
			}
		}

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

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	::DestroyWindow(myWindowHandle);
	//::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void Application::ShowConfig()
{
	myShowConfig = !myShowConfig;
}

void Application::SetupImGuiStyle()
{
	ImGuiStyle & style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.GrabRounding = style.FrameRounding = 2.3f;
}

void Application::ProcessInput()
{

	try
	{
		while(!myClient.IsMessageQueueEmpty())
		{
			Lucent::ChatMessage message = myClient.PopMessage();

			std::cout << "User: " << message.Username << std::endl;
			std::cout << "Broadcaster: " << message.IsBroadcaster << std::endl;
			std::cout << "Moderator: " << message.IsModerator << std::endl;
			std::cout << "VIP: " << message.IsVIP << std::endl;

			std::cout << "Subscriber: " << message.IsSub << std::endl;
			std::cout << "Sub Tier: " << static_cast<int>(message.SubTier) << std::endl;
			std::cout << "Sub Months: " << message.SubMonths << std::endl;

			std::cout << "Chat Channel: " << message.Channel << std::endl;
			std::cout << "Message: " << message.Message << std::endl;

			// HACK: auto join/part
			if(message.Message == "JOIN")
			{
				message.Message = "!join";
			}
			else if(message.Message == "PART")
			{
				message.Message = "!part";
			}

			if(!message.Message.empty())
			{
				HandlePRIVMSG(message);

				for(auto& bot : myBots)
				{
					bot->HandleBotCommands(message);
				}
			}
		}
	}
	catch(...)
	{
		std::cout << "Something went wrong!" << std::endl;
	}

	
}

void Application::Update()
{
	TimerManager::Update();

	for (auto& bot : myBots)
	{
		bot->Update();
	}
}

void Application::Render()
{
	if (mySettings.Headless)
	{
		return;
	}


	RenderConfig();

	ConsoleLog::Get().Draw("Console");

	for(const auto& bot : myBots)
	{
		bot->Draw();
	}
}

void Application::RenderMenu()
{
	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItemEx("Config", myShowConfig ? ICON_FK_CHECK : ""))
			{
				ShowConfig();
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Help"))
		{
			if(ImGui::MenuItem("Help"))
			{
				system("explorer https://github.com/Maikatura/LucentOSC/");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Application::RenderConfig()
{
	if (!myShowConfig)
	{
		return;
	}

	ImGui::Begin("Twitch Settings");

	ImGui::InputText("Twitch Token", &myClient.GetAccountAuth(), ImGuiInputTextFlags_Password);
	ImGui::InputText("Twitch Username", &myClient.GetAccountName());

	ImGui::InputText("##Join", &myJoinChannel);
	ImGui::SameLine();

	if(ImGui::Button("Join"))
	{
		myClient.Join(myJoinChannel);
		myJoinChannel = "";
	}

	auto& joinedChannels = myClient.GetJoinedChannels();
	for(size_t i = 0; i < joinedChannels.size(); i++)
	{
		ImGui::TextWrapped(joinedChannels[i].c_str());
		ImGui::SameLine();
		std::string ButtonWithId = "Part##" + joinedChannels[i];
		if(ImGui::Button(ButtonWithId.c_str()))
		{
			myClient.Part(joinedChannels[i]);
		}
	}

	ImGui::Checkbox("Minimize To Tray", &GetSettings().MinimizeToSystemTray);

	ImGui::End();
}

void Application::HandlePRIVMSG(const Lucent::ChatMessage& priv)
{
	if (!myClient.IsAdmin(priv.Username))
	{
		return;
	}

	auto [first, second] = SplitCommand(priv.Message);
	toLower(second);


	if(first == "!quit")
	{
		myIsRunning = false;
	}
	else if (first == "!addbot" && !second.empty())
	{
		bool exists = false;

		for (const auto& bot : myBots)
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
			myClient.SendChatMessage(priv.Channel, '@' + priv.Username + ' ' + second + " is already running.");
		}
		else
		{
			Bot::Ptr bot = nullptr;

			if (second == "vrchat")
				bot = std::make_unique<VRChat>(myClient);
			else if(second == "chattu")
				bot = std::make_unique<Chattu>(myClient);

			if (bot)
			{
				myBots.emplace_back(std::move(bot));
				myClient.SendChatMessage(priv.Channel, '@' + priv.Username + " Added bot: " + second);
			}
			else
			{
				myClient.SendChatMessage(priv.Channel,'@' + priv.Username + " Unrecognized bot name: " + second);
			}
		}
	}
	else if (first == "!removebot" && !second.empty())
	{
		bool removed = false;

		for (auto it = myBots.begin(); it != myBots.end(); ++it)
		{
			std::string name = typeid(*(*it)).name();
			toLower(name);

			if (endsWith(name, second))
			{
				removed = true;
				myBots.erase(it);
				break;
			}
		}

		if (removed)
		{
			myClient.SendChatMessage(priv.Channel,'@' + priv.Username + " Removed bot: " + second);
		}
		else
		{
			myClient.SendChatMessage(priv.Channel, '@' + priv.Username + " Unrecognized bot name: " + second);
		}
	}
	else if(first == "!addadmin" && !second.empty())
	{
		myClient.AddAdmin(second);
		myClient.SendChatMessage(priv.Channel, "Added '"  +second+ "' as admin");
	}
	else if(first == "!removeadmin" && !second.empty())
	{
		myClient.RemoveAdmin(second);
		myClient.SendChatMessage(priv.Channel,"Removed '" + second + "' as admin");
	}
}

void Application::SetUpdateBuffers(bool cond)
{
	myWantToResizeBuffers = cond;
}

void Application::SetMinimized(bool cond)
{
	myIsMinimized = cond;
}

bool Application::ShowTaskBarButton(bool aVisible)
{
	if(aVisible)
	{
		
		LONG_PTR dwNewLong = GetWindowLongPtr(myWindowHandle, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW;
		SetWindowLongPtr(myWindowHandle, GWL_EXSTYLE, dwNewLong);

		// Update the window's non-client area to reflect the style change
		SetWindowPos(myWindowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		
	}
	else
	{
		// Set the window's extended style to hide the taskbar icon
		LONG_PTR dwNewLong = GetWindowLongPtr(myWindowHandle, GWL_EXSTYLE) | WS_EX_TOOLWINDOW;
		SetWindowLongPtr(myWindowHandle, GWL_EXSTYLE, dwNewLong);

		// Update the window's non-client area to reflect the style change
		SetWindowPos(myWindowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	return TRUE;
}

void Application::SetUpTrayMenu()
{
	AddEntry(Tray::Label(L"LucentOSC"));
	AddEntry(Tray::Separator());

	AddEntry(Tray::Button(L"Open", [&] {
		ShowTaskBarButton(true);
		SetMinimized(false);
		ShowWindow(myWindowHandle, SW_NORMAL);
		}));

	AddEntry(Tray::Toggle(L"Pause", false, [&](bool aState) {

		myProgramIsPaused = aState;

		}));

	AddEntry(Tray::Separator());
	AddEntry(Tray::Button(L"Exit", [&] {

		myIsRunning = false;
		::PostQuitMessage(0);
		}));
}

void Application::UpdateTray()
{
	DestroyMenu(myMenu);
	myMenu = Construct(entries, this, true);

	if(Shell_NotifyIcon(NIM_MODIFY, &notifyData) == FALSE)
	{
		throw std::runtime_error("Failed to update tray icon");
	}

	SendMessage(myWindowHandle, WM_INITMENUPOPUP, reinterpret_cast<WPARAM>(myMenu), 0);
}

HMENU Application::Construct(const std::vector<std::shared_ptr<Tray::TrayEntry>>& entries, Application* parent,
	bool cleanup)
{
	static auto id = 0;
	if(cleanup)
	{
		parent->allocations.clear();
	}

	auto* menu = CreatePopupMenu();
	for(const auto& entry : entries)
	{
		auto* item = entry.get();

		auto name = std::shared_ptr<wchar_t[]>(new wchar_t[item->getText().size() + 1]);
		wcscpy(name.get(), item->getText().c_str()); // NOLINT
		parent->allocations.emplace_back(name);

		MENUITEMINFO winItem{ 0 };

		winItem.wID = ++id;
		winItem.dwTypeData = reinterpret_cast<LPWSTR>(name.get());
		winItem.cbSize = sizeof(MENUITEMINFO);
		winItem.fMask = MIIM_TYPE | MIIM_STATE | MIIM_DATA | MIIM_ID;
		winItem.dwItemData = reinterpret_cast<ULONG_PTR>(item);

		if(auto* toggle = dynamic_cast<Tray::Toggle*>(item); toggle)
		{
			if(toggle->isToggled())
			{
				winItem.fState |= MFS_CHECKED;
			}
			else
			{
				winItem.fState |= MFS_UNCHECKED;
			}
		}
		else if(auto* syncedToggle = dynamic_cast<Tray::SyncedToggle*>(item); syncedToggle)
		{
			if(syncedToggle->isToggled())
			{
				winItem.fState |= MFS_CHECKED;
			}
			else
			{
				winItem.fState |= MFS_UNCHECKED;
			}
		}
		else if(auto* submenu = dynamic_cast<Tray::Submenu*>(item); submenu)
		{
			winItem.fMask |= MIIM_SUBMENU;
			winItem.hSubMenu = Construct(submenu->getEntries(), parent);
		}
		else if(auto* iconButton = dynamic_cast<Tray::ImageButton*>(item); iconButton)
		{
			winItem.fMask = MIIM_STRING | MIIM_BITMAP | MIIM_FTYPE | MIIM_STATE;
			winItem.hbmpItem = iconButton->getImage();
		}
		else if(dynamic_cast<Tray::Label*>(item))
		{
			winItem.fState = MFS_DISABLED;
		}
		else if(dynamic_cast<Tray::Separator*>(item))
		{
			winItem.fType = MFT_SEPARATOR;
		}

		if(!dynamic_cast<Tray::Label*>(item))
		{
			if(item->isDisabled())
			{
				winItem.fState = MFS_DISABLED;
			}
		}

		InsertMenuItem(menu, id, TRUE, &winItem);
	}

	return menu;
}


Settings& Application::GetSettings()
{
	return mySettings;
}

HMENU& Application::GetMenu()
{
	return myMenu;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Application::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static Application* graphicsEnginePtr = nullptr;


	if(ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
		case WM_TRAY:
		{
			if(lParam == WM_RBUTTONUP)
			{
				POINT p;
				GetCursorPos(&p);
				SetForegroundWindow(hWnd);
				auto cmd = TrackPopupMenu(graphicsEnginePtr->GetMenu() , TPM_RETURNCMD | TPM_NONOTIFY, p.x, p.y, 0, hWnd, nullptr);
				SendMessage(hWnd, WM_COMMAND, cmd, 0);
				return 0;
			}
			break;
		}
		case WM_CREATE:
		{
			const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			graphicsEnginePtr = static_cast<Application*>(createdStruct->lpCreateParams);
		}

		case WM_SIZE:
		{
			if(DX11::Device != NULL)
			{
				//graphicsEnginePtr->SetWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));

				switch(wParam)
				{
				case SIZE_MAXIMIZED:
				{
					graphicsEnginePtr->SetMinimized(false);
					graphicsEnginePtr->SetUpdateBuffers(true);
					break;
				}
				case SIZE_MINIMIZED:
				{
					graphicsEnginePtr->SetMinimized(true);
					if(graphicsEnginePtr->GetSettings().MinimizeToSystemTray)
					{
						graphicsEnginePtr->ShowTaskBarButton(false);
						//graphicsEnginePtr->SetUpTrayMenu();
						
						
					}
					break;
				}
				case SIZE_RESTORED:
				{
					graphicsEnginePtr->SetUpdateBuffers(true);
					graphicsEnginePtr->SetMinimized(false);
					break;
				}
				case SIZE_MAXSHOW:
				{
					graphicsEnginePtr->SetUpdateBuffers(true);
					graphicsEnginePtr->SetMinimized(false);
					break;
				}
				}
			}
			return 0;
		}

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}

		case WM_EXITSIZEMOVE:
		{
			if(DX11::Device != NULL)
			{
				graphicsEnginePtr->SetUpdateBuffers(true);
			}
			return 0;
		}

		case WM_SYSCOMMAND:
		{
			if((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			{
				return 0;
			}
			break;
		}

		case WM_DROPFILES:
		{
			std::cout << "Dropped something\n";
			break;
		}

		case WM_QUIT:
		{
			
			std::cout << "Test\n";
			break;
		}

		case WM_CLOSE:
		{
			std::cout << "Dropped something\n";
			break;
		}

		case WM_COMMAND:
		{

			MENUITEMINFO winItem{ 0 };
			winItem.fMask = MIIM_DATA | MIIM_ID;
			winItem.cbSize = sizeof(MENUITEMINFO);

			
			if(GetMenuItemInfo(graphicsEnginePtr->GetMenu(), static_cast<UINT>(wParam), FALSE, &winItem))
			{
				auto* item = reinterpret_cast<Tray::TrayEntry*>(winItem.dwItemData);
				if(auto* button = dynamic_cast<Tray::Button*>(item); button)
				{
					button->clicked();
				}
				else if(auto* toggle = dynamic_cast<Tray::Toggle*>(item); toggle)
				{
					toggle->onToggled();
					graphicsEnginePtr->UpdateTray();
				}
				else if(auto* syncedToggle = dynamic_cast<Tray::SyncedToggle*>(item); syncedToggle)
				{
					syncedToggle->onToggled();
					graphicsEnginePtr->UpdateTray();
				}
			}
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
