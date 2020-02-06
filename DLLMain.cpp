#pragma comment( lib, "Winmm.lib" )

#include <Windows.h>
#include <stdio.h>

#include "InterfaceManager.h"
#include "HookManager.h"
#include "netvars.h"
#include "RenderUtils.h"
#include "Menu.h"

#include "SecurityUtils.h"
#include "LoggerUtils.h"

#include "Menu.h"

#include <chrono>
#include <thread>

WNDPROC o_windowproc = NULL;
HINSTANCE hinstance;

LRESULT __stdcall window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool should_return_false = false;
	bool click_button_press;

	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		MenuValues::MenuOpen = false;
		break;
	case WM_XBUTTONDOWN:
		Menu::HandleKeyInput(GetAsyncKeyState(VK_XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2);
		break;
	case WM_KEYDOWN:
		Menu::HandleKeyInput(wParam);
		break;
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		click_button_press = true;
		break;
	default:
		break;
	}

	if (MenuUtils::VirtualToNumber(wParam).length() > 0 && game_interfaces::Engine->InGame() || click_button_press)
	{
		should_return_false = true;
	}

	if (MenuValues::MenuOpen && should_return_false)
	{
		return FALSE;
	}

	return CallWindowProc(o_windowproc, hwnd, uMsg, wParam, lParam);
}

#define DEBUG true

int thread()
{
	Logger::SaveToLog("--------------------Dominance Log [Start]--------------------");
	/*SecurityUtils::SetData();*/
	Logger::SaveToLog("General check...");
	//SecurityUtils::check_guard();

	while (!GetModuleHandleA("serverbrowser.dll"))
	{
		//SecurityUtils::check_guard();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (DEBUG)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		SetConsoleTitle("Console");
	}


	/*SecurityUtils::check_guard();*/
	Logger::SaveToLog("Game loaded now loading interfaces");
	game_interfaces::init();
	Logger::SaveToLog("Loading netvars");
	netvars->Initialize();
	Logger::SaveToLog("Loading menu");
	Menu::Initialize((HWND)hinstance);
	Logger::SaveToLog("Loading hooks");
	hook_manager::init();
	Logger::SaveToLog("Dominance loaded.");

	return EXIT_SUCCESS;
}

int keyboard_thread()
{
	o_windowproc = (WNDPROC)SetWindowLongPtr(FindWindowA("Valve001", "Counter-Strike: Global Offensive"), GWL_WNDPROC, (LONG_PTR)window_proc);
	return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,_In_ DWORD fdwReason,_In_ LPVOID lpvReserved) 
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hinstance = hinstDLL;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyboard_thread, NULL, NULL, NULL);
	 	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread, NULL, NULL, NULL);
		break;
	default:
		break;
	}

	return TRUE;
}
