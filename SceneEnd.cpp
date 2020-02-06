#include "HookManager.h"
#include "Menu.h"
#include "Visuals.h"
#include "Glow.h"
#include "LagFix.h"

SceneEndFn hooks::oSceneEnd;

void __fastcall hooks::SceneEnd_Hook(void* thisptr, void* edx)
{
	hooks::oSceneEnd(thisptr, edx); // Original function

	if (game_interfaces::Engine->Connected() && game_interfaces::Engine->InGame() && !game_interfaces::Engine->IsTakingScreenshot())
	{
		if (MenuControls::v_players_esp_enable->GetState())
		{
			if (MenuControls::v_players_esp_ally_glow->GetState() || MenuControls::v_players_esp_enemy_glow->GetState())
			{
				Glow::Draw();
			/*	Glow::Think();*/
			}
		}	
		Visuals::SceneEnd_Render(); // Chams
	}
}