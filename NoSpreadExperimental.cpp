#include "Resolver.h"

#include "Menu.h"
#include "Visuals.h"

#include "Aimbot.h"

void NoSpreadResolver_Experimental::Main_Update(entity* ent, ResolverPlayer &res, entity* local)
{
	if (Resolver::TargetValid(ent) && local)
	{
		if (res.NoSpread_Experimental.R_LastLowerBody != ent->m_flLowerBodyYawTarget())
		{
			res.NoSpread_Experimental.ResolverAngle = ent->m_flLowerBodyYawTarget();
			res.NoSpread_Experimental.ResolveMode = NoSpread_LBY;

			res.NoSpread_Experimental.R_LowerbodyLog.push_back(ent->m_flLowerBodyYawTarget());
			res.NoSpread_Experimental.R_LastLowerBody = ent->m_flLowerBodyYawTarget();
		}
		else {
			bool continue_ = true;
			if (res.NoSpread_Brute.R_LastHitTimer > 0)
			{
				res.NoSpread_Brute.R_LastHitTimer -= 1;

				res.NoSpread_Experimental.ResolverAngle = res.NoSpread_Brute.R_LastHittedAngle;
				res.NoSpread_Experimental.ResolveMode = NoSpread_BRUTEFORCE_LASTHIT;
			}
			else {
				if (res.NoSpread_Experimental.R_LowerbodyLog.size() >= 3)
				{
					float diff = 0;
					float last_lby = 0;
					bool init = false;
					bool is_static = true;

					for (float ang : res.NoSpread_Experimental.R_LowerbodyLog)
					{
						if (init)
						{
							float delta = fabs(ang - last_lby);
							if (fabs(diff - delta) > 25)
							{
								is_static = false;
							}
						}
						else {
							init = true;
						}
						last_lby = ang;
					}

					if (is_static)
					{
						res.NoSpread_Experimental.ResolverAngle = ent->m_flLowerBodyYawTarget();
						res.NoSpread_Experimental.ResolveMode = NoSpread_BRUTEFORCE_STATIC;
						continue_ = false;
					}
				}

				if (continue_ && local->m_iHealth() > 0 && !local->IsDormant())
				{
					if (res.NoSpread_Experimental.R_Bruteforce_timer < 15.f)
					{
						res.NoSpread_Experimental.R_Bruteforce_timer += 1.f;
					}
					else {
						res.NoSpread_Experimental.R_Bruteforce_step += 1;
						if (res.NoSpread_Experimental.R_Bruteforce_step > 2)
						{
							res.NoSpread_Experimental.R_Bruteforce_step = 0;
						}
						res.NoSpread_Experimental.R_Bruteforce_timer = 0;
					}

					Vector Local_HeadHitBox;
					if (local->GetHitboxPosition(0, Local_HeadHitBox))
					{
						Vector Result, Delta = Local_HeadHitBox - ent->GetEyePosition();
						MathUtils::VectorAngles(Delta, Result);
						float angle_result = 0.f;
						switch (res.NoSpread_Experimental.R_Bruteforce_step)
						{
						case 0:
							angle_result = Result.y - 100.f;
							break;
						case 1:
							angle_result = Result.y + 100.f;
							break;
						case 2:
							angle_result = Result.y - 180.f;
							break;
						}
						res.NoSpread_Experimental.ResolveMode = NoSpread_BRUTEFORCE_TOTAL;
						res.NoSpread_Experimental.ResolverAngle = angle_result;
					}
				}
			}
		}
	}
}

void NoSpreadResolver_Experimental::Draw(entity* ent)
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

					switch (res.NoSpread_Experimental.ResolveMode)
					{
					case NoSpread_LBY:
						ResolveMode = "LBY";
						break;
					case NoSpread_BRUTEFORCE_STATIC:
						ResolveMode = "STATIC";
						break;
					case NoSpread_BRUTEFORCE_TOTAL:
						ResolveMode = "TOTAL";
						break;
					case NoSpread_BRUTEFORCE_LASTHIT:
						ResolveMode = "HIT";
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

