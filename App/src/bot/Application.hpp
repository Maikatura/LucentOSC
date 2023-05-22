#pragma once

#include "Bot.hpp"


#include "Settings.hpp"
#include "Twitch/TwitchApi.h"

class Application
{
public:
	Application();
	Application(Lucent::TwitchApi& client);
	Application(const Application& aApplication);

	bool Run();
	bool IsRunning();
	void Stop();


private:
	void SetupImGuiStyle();

	void ProcessInput();
	void Update();
	void Render();

	void HandlePRIVMSG(const Lucent::ChatMessage& priv);
	void SetUpdateBuffers(bool cond);
	void SetMinimized(bool cond);
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

private:

	HWND myWindowHandle{};

	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
	bool myIsRunning = true;

	Settings mySettings;
	std::string myJoinChannel;
	Lucent::TwitchApi& myClient;
	std::vector<Bot::Ptr> myBots;
};
