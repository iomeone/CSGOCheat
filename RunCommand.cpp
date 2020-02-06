#include "HookManager.h"
#include "Menu.h"
#include "Entity.h"

void __stdcall hooks::RunCommand_Hook(entity* ent, CUserCmd* cmd, void* move_helper)
{
	if (game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		if (!game_interfaces::MoveHelper) { // If nullptr
			game_interfaces::MoveHelper = (IMoveHelper*)move_helper;
		}
		if (game_interfaces::MoveHelper) { // If not nullptr
			hooks::Prediction_Hook->UnHook(); // We remove the hook
		}
	}
}