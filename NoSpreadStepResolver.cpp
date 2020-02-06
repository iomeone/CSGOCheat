#include "Resolver.h"

#include "Menu.h"
#include "Visuals.h"

void NoSpreadResolver_Step::Draw(entity* ent)
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

					switch (res.NoSpread_Step.ResolveMode)
					{
					case NoSpread_ResolverState::NoSpread_BRUTEFORCE_STEP:
						ResolveMode += "STEPS ";
						ResolveMode += std::to_string(res.NoSpread_Step.R_LastStep);
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

void NoSpreadResolver_Step::Main_Update(entity* ent, ResolverPlayer &res, entity* local)
{
	if (Resolver::TargetValid(ent))
	{
		res.NoSpread_Step.ResolveMode = NoSpread_ResolverState::NoSpread_BRUTEFORCE_STEP;

		if (res.NoSpread_Step.R_Bruteforce_Timer > 35)
		{
			res.NoSpread_Step.R_LastStep += MenuControls::a_resolver_bruteforce_angle_step->GetValue();
			res.NoSpread_Step.R_Bruteforce_Timer = 0;
		}
		else {
			res.NoSpread_Step.R_Bruteforce_Timer += 1;
		}

		if (res.NoSpread_Step.R_LastStep > 180)
			res.NoSpread_Step.R_LastStep = -180;

		res.NoSpread_Step.ResolverAngle = res.NoSpread_Step.R_LastStep;
	}
}