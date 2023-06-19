#pragma once

#include "Bot.hpp"


#include "Settings.hpp"
#include "core/windows/tray.hpp"
#include "Twitch/TwitchApi.h"

static constexpr auto WM_TRAY = WM_USER + 1;

class Application
{
public:
	Application();
	Application(Lucent::TwitchApi& client);
	Application(const Application& aApplication);

	bool Run();
	bool IsRunning();
	void Stop();
	void ShowConfig();
	void UpdateTray();

private:
	void SetupImGuiStyle();

	void ProcessInput();
	void Update();
	void Render();
	void RenderMenu();
	void RenderConfig();

	void HandlePRIVMSG(const Lucent::ChatMessage& priv);
	void SetUpdateBuffers(bool cond);
	void SetMinimized(bool cond);

	bool ShowTaskBarButton(bool aVisible);

	void SetUpTrayMenu();
	

	template <typename T, std::enable_if_t<std::is_base_of<Tray::TrayEntry, T>::value>* = nullptr>
	auto AddEntry(const T& entry);


	static HMENU Construct(const std::vector<std::shared_ptr<Tray::TrayEntry>>& entries, Application* parent, bool cleanup = false);

	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	Settings& GetSettings();

	HMENU& GetMenu();

private:

	HWND myWindowHandle{};
	HMENU myMenu = nullptr;
	NOTIFYICONDATA notifyData {};

	bool myShowConfig = false;
	bool myProgramIsPaused = false;
	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
	bool myIsRunning = true;
	bool myNeedToBeChannelModToBeAdmin = true;

	Settings mySettings;
	std::string myJoinChannel;
	Lucent::TwitchApi& myClient;
	std::vector<Bot::Ptr> myBots;
	std::map<HWND, std::reference_wrapper<Application>> trayList;

	std::vector<std::shared_ptr<Tray::TrayEntry>> entries;
	std::vector<std::shared_ptr<wchar_t[]>> allocations;
};

template <typename T, std::enable_if_t<std::is_base_of<Tray::TrayEntry, T>::value>*>
auto Application::AddEntry(const T& entry)
{
	entries.emplace_back(std::make_shared<T>(entry));
	auto back = entries.back();
	back->setParent(this);
	UpdateTray();

	return std::dynamic_pointer_cast<std::decay_t<T>>(back);
}
