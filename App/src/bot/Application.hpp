#pragma once

#include "Bot.hpp"


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
	void Update();
	void Render();

	void HandlePRIVMSG(const PRIVMSG& priv);
	void SetUpdateBuffers(bool cond);
	void SetMinimized(bool cond);
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

private:

	HWND myWindowHandle{};

	Settings mySettings;
	Client& m_client;
	bool myWantToResizeBuffers = false;
	bool myIsMinimized = false;
	bool myIsRunning = true;
	std::vector<Bot::Ptr> m_bots;
};
