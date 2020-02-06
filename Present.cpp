#include "HookManager.h"
#include "InterfaceManager.h"
#include "MiscFunctions.h"
#include "RenderUtils.h"
#include "Visuals.h"
#include "Menu.h"
#include "LagFix.h"
#include "Aimbot.h"
#include <d3d9.h>
#include "LoggerUtils.h"
#include "Menu.h"
#include "SecurityUtils.h"

PresentFn hooks::oPresent;

float update_timer;

bool inited = false;

HRESULT __stdcall hooks::Present_Hook(LPDIRECT3DDEVICE9 Device, RECT* SourceRect, RECT* DestRect, HWND DestWindowOverride, RGNDATA* DirtyRegion)
{
	if (game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		RenderUtils::directx_device = Device;
		if (RenderUtils::directx_device && !inited)
		{
			RenderUtils::InitCheatTitle(RenderUtils::directx_device);
			RenderUtils::LoadFonts();
			inited = true;
		}	
		RenderUtils::directx_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
		if (RenderUtils::Normal_font)
		{
			if (!game_interfaces::Engine->IsTakingScreenshot())
			{
				if (game_interfaces::Engine->Connected() && game_interfaces::Engine->InGame())
				{
					Misc_Functions::ThirdPerson();
					Visuals::PaintTraverse_Render(); // ESP box, health, resolver, backtrack								
				}
				Logger::Render();
				Menu::Update();
				Menu::Render();
			}
		}
	}
	return hooks::oPresent(Device, SourceRect, DestRect, DestWindowOverride, DirtyRegion);
}