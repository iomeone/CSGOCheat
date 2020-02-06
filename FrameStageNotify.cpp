#include "HookManager.h"
#include "LagCompensation.h"
#include "InterfaceManager.h"
#include "Menu.h"
#include "Resolver.h"
#include "Aimbot.h"
#include "LagFix.h"
#include "SkinChanger.h"
#include "MiscFunctions.h"
#include "AntiAim.h"

Vector HookValues::RealAimPunchAngle;
FrameStageNotifyFn hooks::oFrameStageNotify;

void  __stdcall hooks::FrameStageNotify_Hook(ClientFrameStage_t stage)
{
	if (game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		static int* SmokeCount = *(int**)(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "8B 1D ? ? ? ? 56 33 F6 57 85 DB") + 2);
		if (MenuControls::v_other_nosmoke->GetSelectedItem() > 0)
		{
			*SmokeCount = 0;
		}

		entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());

		if (stage == FRAME_RENDER_START)
		{
			if (local)
			{
				if (!local->IsDormant() && local->m_iHealth() > 0)
				{
					//// We invalidate occlusion for all the players
					//for (int i = 0; i <= game_interfaces::Engine->GetMaxClients(); i++)
					//{
					//	entity* ent = game_interfaces::EntityList->GetClientEntity(i);
					//	if (i == game_interfaces::Engine->GetLocalPlayer() || !ent)
					//		continue;

					//	if (ent->IsPlayer() && !ent->IsDormant() && ent->m_iHealth() > 0)
					//	{
					//		ent->InvalidateOcclusion();
					//	}
					//}
					
					// AntiAim Visualization
					if (Misc_Functions::ThirdPersonSwitch)
					{
						game_interfaces::Prediction->SetLocalViewAngles(AntiAim::AntiAimAngles_Real);
					}

					// No Visual Recoil
					if (MenuControls::a_norecoil->GetState())
					{
						HookValues::RealAimPunchAngle = *local->m_aimPunchAngle();
						*local->m_aimPunchAngle() = Vector(0, 0, 0);
						*local->m_viewPunchAngle() = Vector(0, 0, 0);
					}
				}
			}
		}
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			LagFix::Update();
		
			if (MenuControls::v_skins_enable->GetState())
			{
				SkinChanger::UpdateAll();
			}

			entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
			baseweapon* localweapon = local->GetWeapon(); // We get the weapon from the localplayer
			if (local && localweapon)
			{
				LC::MaxTicks = (TIME_TO_TICKS((float)MenuControls::m_fakelatency->GetValue() / 1000.f) - TIME_TO_TICKS(LC::GetPing() / 1000.f)) + 12.f;
				for (int i = 0; i < game_interfaces::Globals->maxClients; i++)
				{
					if (i == game_interfaces::Engine->GetLocalPlayer())
						continue;
					entity* ent = game_interfaces::EntityList->GetClientEntity(i);
					if (ent)
					{
						if (ent->IsPlayer() && !ent->IsDormant() && ent->m_iHealth() > 0)
						{
							if (MenuControls::a_backtrack->GetState())
							{
								if (ent->m_iTeamNum() != local->m_iTeamNum())
								{
									LC::UpdateBackTrackEntity(ent, local);
								}
							}
							if (MenuControls::a_resolver->GetState())
							{
								Resolver::Resolve(ent);
								Resolver::Set(local, ent);
							}
						}
					}
				}
			}	
		}
	}
	hooks::oFrameStageNotify(stage); // Original function
}