#include "Application.hpp"
#include "Utility.hpp"

// bots
#include "Bot/Jukebox.hpp"
#include <Windows.h>
#include <dwmapi.h>

#include "FileChecker.hpp"



#include "misc/TimerManager.h"

#include "imgui.h"
#include "Bot/Kick/Kick.h"
#include "Bot/VRChat/VRChat.h"
#include "Bot/Twitch/Chattu.h"
#include "Bot/Discord/Discord.h"

#include <shellapi.h>

#include "Framework/DX11.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#pragma comment(lib, "dwmapi.lib")

Lucent::TwitchApi nullClient;

inline bool ExistsTest(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



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

	ImGui_ImplWin32_Init(myWindowHandle);
	ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());

	// Setup Dear ImGui context
	
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	

	m_bots.emplace_back(std::make_unique<VRChat>(myClient));
	m_bots.emplace_back(std::make_unique<Chattu>(myClient));
	m_bots.emplace_back(std::make_unique<Discord>(myClient));
	m_bots.emplace_back(std::make_unique<Kick>(myClient));
}

Application::Application(const Application& aApplication) : Application(aApplication.myClient)
{
}

bool Application::Run()
{
	
	MSG msg = { 0 };

	// Main loop
	while(myIsRunning)
	{

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

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

		/*for(auto& layer : myLayerStack)
			layer->OnUpdate(myTimeStep);*/

	
		if(!myIsMinimized)
		{

			// Start the Dear ImGui frame
			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_NewFrame();
			ImGui::NewFrame();

			if(myWantToResizeBuffers)
			{
				if(DX11::SwapChain)
				{
					DX11::Resize();
				}
				myWantToResizeBuffers = false;
			}



			std::array<float, 4> clear_color[4] = { 0.0f, 0.0f,0.0f,1.0f };

			DX11::BeginFrame(clear_color[0]);

			Update();
			ProcessInput();

			{
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
				

				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

				// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
				// and handle the pass-thru hole, so we ask Begin() to not render a background.
				if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
				// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
				// all active windows docked into it will lose their parent and become undocked.
				// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
				// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace Demo", nullptr, window_flags);
				ImGui::PopStyleVar();

				ImGui::PopStyleVar(2);

				// Submit the DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("VulkanAppDockspace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				
				if(ImGui::BeginMenuBar())
				{

					if (ImGui::BeginMenu("Hello"))
					{
						
					}

					ImGui::EndMenuBar();
				}

				Render();

				ImGui::End();
			}


			// Rendering
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			ImGuiIO& io = ImGui::GetIO();
			// Update and Render additional Platform Windows
			if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			/*float time = 0.0f;
			myFrameTime = time - myLastFrameTime;
			myLastFrameTime = time;*/

			DX11::EndFrame();

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
}

void Application::ProcessInput()
{
	

	while(!myClient.IsMessageQueueEmpty())
	{
		Lucent::ChatMessage message = myClient.PopMessage();


		std::cout << "User: " << message.Username << std::endl;
		std::cout << "Broadcaster: " << message.IsBroadcaster << std::endl;
		std::cout << "Moderator: " << message.IsModerator << std::endl;
		std::cout << "VIP: " << message.IsVIP << std::endl;
		std::cout << "Subscriber: " << message.IsSub << std::endl;
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

		if(!message.Message.empty() && message.Message[0] == '!')
		{
			HandlePRIVMSG(message);

			for(auto& bot : m_bots)
			{
				bot->HandlePRIVMSG(message);
			}
		}
	}
}

void Application::Update()
{

	TimerManager::Update();

	for (auto& bot : m_bots)
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

	
	for(const auto& bot : m_bots)
	{
		bot->Draw();
	}
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
				m_bots.emplace_back(std::move(bot));
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

LRESULT CALLBACK Application::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static Application* graphicsEnginePtr = nullptr;


	if(ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
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
						graphicsEnginePtr->SetUpdateBuffers(true);
						graphicsEnginePtr->SetMinimized(false);
						break;

					case SIZE_MINIMIZED:
						graphicsEnginePtr->SetMinimized(true);
						break;

					case SIZE_RESTORED:
						graphicsEnginePtr->SetUpdateBuffers(true);
						graphicsEnginePtr->SetMinimized(false);
						break;

					case SIZE_MAXSHOW:
						graphicsEnginePtr->SetUpdateBuffers(true);
						graphicsEnginePtr->SetMinimized(false);
						break;
				}
			}
		}

		case WM_EXITSIZEMOVE:
		{
			if(DX11::Device != NULL)
			{
				graphicsEnginePtr->SetUpdateBuffers(true);
			}
		}

		case WM_SYSCOMMAND:
		{
			if((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			{
				return 0;
			}
			break;
		}



		case WM_DESTROY:
		{
			PostQuitMessage(0);
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

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
