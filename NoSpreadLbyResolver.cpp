#include "Resolver.h"
#include "Aimbot.h"
#include "Menu.h"
#include "Visuals.h"

void NoSpreadResolver_LBY::Draw(entity* ent)
{
	if (MenuControls::m_debug->GetState())
	{
		if (Resolver::TargetValid(ent))
		{
			bool AlreadyInList = false;
			for (ResolverPlayer &res : Resolver::Resolver_Players)
			{
				if (res.PlayerIndex == ent->GetIndex())
				{
					std::string ResolveMode = "";

					switch (res.NoSpread_LBY.ResolveMode)
					{
					case NoSpread_ResolverState::NoSpread_BRUTEFORCE_LASTHIT:
						ResolveMode += "HIT";
						break;
					case NoSpread_ResolverState::NoSpread_BRUTEFORCE_EXPERIMENTAL:
						ResolveMode += "EXPERIMENTAL";
						break;
					default:
						ResolveMode = "";
						break;
					}

					Visuals::ESPInfo_List.push_back(ESPInfo(ent->GetIndex(), ResolveMode));
				}
			}
		}
	}
}

void NoSpreadResolver_LBY::Main_Update(entity* ent, ResolverPlayer &res, entity* local)
{
	if (Resolver::TargetValid(ent))
	{
		bool local_over_ent = (local->GetAbsOrigin().z - ent->GetAbsOrigin().z) > 15.f;
		bool on_ground = ent->m_fFlags() & FL_ONGROUND;

		if (on_ground)
		{
			res.NoSpread_LBY.R_LastGroundLby = ent->m_flLowerBodyYawTarget();
		}

		if (res.NoSpread_Brute.R_LastHitTimer > 0)
		{
			res.NoSpread_Brute.R_LastHitTimer -= 1;

			res.NoSpread_LBY.ResolverAngle = res.NoSpread_Brute.R_LastHittedAngle;
			res.NoSpread_Brute.ResolveMode = NoSpread_BRUTEFORCE_LASTHIT;
		}
		else {
			res.NoSpread_LBY.ResolverAngle = res.NoSpread_LBY.R_LastGroundLby;
			res.NoSpread_Brute.ResolveMode = NoSpread_BRUTEFORCE_EXPERIMENTAL;
		}
	}
}

