#include "Resolver.h"

#include "InterfaceManager.h"
#include "Menu.h"
#include "Aimbot.h"

std::vector<ResolverPlayer> Resolver::Resolver_Players;

bool Resolver::TargetValid(entity* ent) // If entity is valid
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local && ent)
	{
		if (ent->m_iHealth() > 0 && !ent->IsDormant() && ent->IsPlayer() && !ent->m_bGunGameImmunity())
		{
			if (ent->GetIndex() != game_interfaces::Engine->GetLocalPlayer())
			{
				if (ent->m_iTeamNum() != local->m_iTeamNum() || MenuControls::a_resolver_resolve_all->GetState())
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Resolver::Draw(entity* ent)
{
	if (TargetValid(ent))
	{
		switch (MenuControls::a_resolver_mode->GetSelectedItem())
		{
		case 0: // Experimental
			NoSpreadResolver_Experimental::Draw(ent);
			break;
		case 1: // Ground
			SpreadResolver::Draw(ent);
			break;
		case 2: // Brute
			NoSpreadResolver_View::Draw(ent);
			break;
		case 3: // Step
			NoSpreadResolver_Step::Draw(ent);
			break;
		case 4: // LBY
			NoSpreadResolver_LBY::Draw(ent);
			break;
		default:
			break;
		}
	}
}

#include "LagCompensation.h"

void Resolver::Set(entity* local, entity* ent)
{
	if (TargetValid(ent))
	{
		for (ResolverPlayer &res : Resolver::Resolver_Players)
		{
			if (res.PlayerIndex == ent->GetIndex())
			{
				res.R_VisibleAngle = ent->m_angEyeAngles();

				Vector* eye = ent->m_angEyeAngles_ptr();
				Vector resolver_ang;
			

				switch (MenuControls::a_resolver_mode->GetSelectedItem())
				{
				case 0: // Experimental
					resolver_ang = Vector(0, res.NoSpread_Experimental.ResolverAngle, 0);
					break;
				case 1: // Ground
					resolver_ang = Vector(0, res.Spread.ResolverAngle, 0);
					break;
				case 2: // Brute
					resolver_ang = Vector(0, res.NoSpread_Brute.ResolverAngle, 0);
					break;
				case 3: // Step
					resolver_ang = Vector(0, res.NoSpread_Step.ResolverAngle, 0);
					break;
				case 4: // LBY
					resolver_ang = Vector(0, res.NoSpread_LBY.ResolverAngle, 0);
					break;
				default:
					break;
				}

				MathUtils::AntiUntrusted_ClampAngles(resolver_ang);

				eye->y = resolver_ang.y;

				if (res.R_PitchTimer > 25)
				{
					res.R_PitchMode += 1;
					if (res.R_PitchMode > 2)
					{
						res.R_PitchMode = 0;
					}
					res.R_PitchTimer = 0;
				}
				else {
					res.R_PitchTimer += 1;
				}

				switch (MenuControls::a_resolver_overwrite_pitch->GetSelectedItem())
				{
				case 0: // None
					break;
				case 1: // Down
					eye->x = 89.f;
					break;
				case 2: // Zero
					eye->x = 0.f;
					break;
				case 3: // Mix
					switch (res.R_PitchMode)
					{
					case 0:
						eye->x = 89.f;
						break;
					case 1:
						eye->x = 0;
						break;
					case 2:
						eye->x = -89.f;
						break;
					default:
						break;
					}
					break;
				case 4: // Up
					eye->x = -89.f;
					break;
				default:
					break;
				}
			}
		}
	}
}

bool Resolver::IsUnresolved(entity* ent)
{
	if (TargetValid(ent))
	{
		for (ResolverPlayer &res : Resolver_Players)
		{
			if (res.PlayerIndex == ent->GetIndex())
			{
				switch (MenuControls::a_resolver_mode->GetSelectedItem())
				{
				case 1: // Spread
					if (/*res.Spread.ResolveMode == Spread_BRUTEFORCE ||*/ res.Spread.ResolveMode == Spread_BREAKING_LBY_UNDER_120 || res.Spread.ResolveMode == Spread_BREAKING_LBY_OVER_120)
					{
						return true;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	return false;
}

void Resolver::Resolve(entity* ent)
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (TargetValid(ent) && local)
	{
		bool AlreadyInList = false;
		for (ResolverPlayer &res : Resolver_Players)
		{
			if (res.PlayerIndex == ent->GetIndex())
			{
				AlreadyInList = true;

				switch (MenuControls::a_resolver_mode->GetSelectedItem())
				{
				case 0: // Experimental
					NoSpreadResolver_Experimental::Main_Update(ent, res, local);
					break;
				case 1: // Ground
					//SpreadResolver::Second_Update(ent, res);
					SpreadResolver::Main_Update(ent, res, local);
					SpreadResolver::LBY_Update(ent, res);
					break;
				case 2: // Brute
					NoSpreadResolver_View::Main_Update(ent, res, local);
					break;
				case 3: // Step
					NoSpreadResolver_Step::Main_Update(ent, res, local);
					break;
				case 4: // LBY
					NoSpreadResolver_LBY::Main_Update(ent, res, local);
					break;
				default:
					break;
				}
			}
		}
		if (!AlreadyInList && ent->GetIndex() != game_interfaces::Engine->GetLocalPlayer())
		{
			Resolver_Players.push_back(ResolverPlayer(ent->GetIndex())); // New Player				
		}
	}
}