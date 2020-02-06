#include "HookManager.h"
#include "InterfaceManager.h"

GetLastTimeStampFn hooks::oGetLastTimeStamp;
float HookValues::interpolation_disable_timer;

float __stdcall hooks::GetLastTimeStamp_Hook()
{
	if (game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		if (HookValues::interpolation_disable_timer > 25) // If we do this 24/7 we get some huge fps drops, so why not doing it only every second? 
		{
			for (int i = 0; i <= game_interfaces::Globals->maxClients; i++)
			{
				entity* ent = game_interfaces::EntityList->GetClientEntity(i);
				if (ent)
				{
					if (ent->IsPlayer()) // If entity is player
					{
						if (!ent->IsDormant() && ent->m_iHealth() > 0)
						{
							ent->DisableInterpolation();
						}
					}
				}
			}
			HookValues::interpolation_disable_timer = 0;
		}
		else {
			HookValues::interpolation_disable_timer += 1.f;
		}
	}
	return hooks::oGetLastTimeStamp();
}