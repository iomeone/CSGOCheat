#include "HookManager.h"
#include "Menu.h"

#include "MiscFunctions.h"
#include "Visuals.h"

PaintTraverseFn hooks::oPaintTraverse;
LockCursorFn hooks::oLockCursor;

void __stdcall hooks::LockCursor_Hook()
{
	if (!MenuValues::MenuOpen)
	{
		hooks::oLockCursor(game_interfaces::Surface);
	}
}

void __stdcall hooks::PaintTraverse_Hook(vgui::VPANEL Panel, bool forceRepaint, bool allowForce)
{
	if (!strcmp(game_interfaces::VGui->GetName(Panel), "MatSystemTopPanel"))
	{
		//game_interfaces::VGui->SetMouseInputEnabled(Panel, MenuValues::MenuOpen);
	}

	if (MenuControls::v_other_noscope->GetState())
	{
		if (!(strcmp(game_interfaces::VGui->GetName(Panel), "HudZoom") == 0))
		{
			hooks::oPaintTraverse(game_interfaces::VGui, Panel, forceRepaint, allowForce);
		}
	}
	else {
		hooks::oPaintTraverse(game_interfaces::VGui, Panel, forceRepaint, allowForce);
	}
}