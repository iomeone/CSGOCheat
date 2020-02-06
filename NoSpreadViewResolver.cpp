#include "Resolver.h"

#include "InterfaceManager.h"
#include "Menu.h"
#include "Visuals.h"

void NoSpreadResolver_View::Draw(entity* ent)
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

					switch (res.NoSpread_Brute.ResolveMode)
					{
					case NoSpread_ResolverState::NoSpread_LBY:
						ResolveMode += "LBY";
						break;
					case NoSpread_ResolverState::NoSpread_BRUTEFORCE_LASTHIT:
						ResolveMode += "HIT";
						break;
					case NoSpread_ResolverState::NoSpread_BRUTEFORCE_HEIGHT:
						ResolveMode += "BRUTEFORCE HEIGHT";
						break;
					case NoSpread_ResolverState::NoSpread_BRUTEFORCE_NORMAL:
						ResolveMode += "BRUTEFORCE NORMAL";
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

int LastWeaponShotsFired;

void NoSpreadResolver_View::Main_Update(entity* ent, ResolverPlayer &res, entity* local)
{
	if (Resolver::TargetValid(ent))
	{
		if (local->m_iShotsFired() != LastWeaponShotsFired || res.NoSpread_Brute.R_Bruteforce_Timer > 25.f)
		{
			res.NoSpread_Brute.R_Bruteforce_Mode += 1;
			res.NoSpread_Brute.R_Bruteforce_Timer = 0;
			LastWeaponShotsFired = local->m_iShotsFired();
		}
		else {
			res.NoSpread_Brute.R_Bruteforce_Timer += 1;
		}


		float angle_result = 0.f;

		if (local->m_iHealth() > 0)
		{
			bool local_over_ent = (local->GetAbsOrigin().z - ent->GetAbsOrigin().z) > 15.f;
			Vector Local_HeadHitBox;
			if (local->GetHitboxPosition(Hitboxes::HITBOX_HEAD, Local_HeadHitBox))
			{
				if (res.NoSpread_Brute.R_LastHitTimer > 0)
				{
					angle_result = res.NoSpread_Brute.R_LastHittedAngle;
					res.NoSpread_Brute.R_LastHitTimer -= 1;
					res.NoSpread_Brute.ResolveMode = NoSpread_BRUTEFORCE_LASTHIT;
				}
				else {
					if (local_over_ent)
					{
						res.NoSpread_Brute.ResolveMode = NoSpread_ResolverState::NoSpread_BRUTEFORCE_HEIGHT;

						switch (res.NoSpread_Brute.R_Bruteforce_Mode)
						{
						case 0:
							angle_result = ent->m_flLowerBodyYawTarget();
							break;
						case 1:
							angle_result = ent->m_flLowerBodyYawTarget() - 90.f;
							break;
						case 2:
							angle_result = ent->m_flLowerBodyYawTarget() - 179.f;
							break;
						case 3:
							angle_result = ent->m_flLowerBodyYawTarget() + 90.f;
							break;
						}
					}
					else {
						res.NoSpread_Brute.ResolveMode = NoSpread_ResolverState::NoSpread_BRUTEFORCE_NORMAL;

						Vector Result, Delta = Local_HeadHitBox - ent->GetEyePosition();
						MathUtils::VectorAngles(Delta, Result);

						switch (res.NoSpread_Brute.R_Bruteforce_Mode)
						{
						case 0:
							angle_result = Result.y;
							break;
						case 1:
							angle_result = Result.y + 90;
							break;
						case 2:
							angle_result = Result.y - 90;
							break;
						}
					}
				}
			}
		}

		if (res.NoSpread_Brute.R_Bruteforce_Mode > 3)
		{
			res.NoSpread_Brute.R_Bruteforce_Mode = 0;
		}

		res.NoSpread_Brute.ResolverAngle = angle_result;
	}
}
