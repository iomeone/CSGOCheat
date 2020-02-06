#include "Aimbot.h"
#include "InterfaceManager.h"
#include <iostream>
#include "MathUtils.h"
#include "AutoWall.h"
#include "AntiAim.h"
#include "Menu.h"
#include "RandomStuff.h"
#include "Visuals.h"
#include "LagFix.h"
#include "Resolver.h"

#include "LoggerUtils.h"
//TODO: ADD DISTANCE CHECK (RANGE)


BackTrackRecord Aimbot::BackTrack_Record;
baseweapon* LastWeaponUsed;

int Aimbot::CurrentTarget = -1;
int Aimbot::ShotsFired;
int Aimbot::LastBullets;
bool Aimbot::AimbotShooting;
int Aimbot::AutoRevolverStopTimer;
bool Aimbot::AutoRevolverWorking;
bool Aimbot::UseBackTrack;
bool Aimbot::Extrapolating;

std::vector<int> Aimbot::VitalHitboxes() 
{ 
	std::vector<int> list = std::vector<int>{ HITBOX_HEAD, HITBOX_PELVIS, HITBOX_LEFT_FOOT, HITBOX_RIGHT_FOOT };
	return list;
};

std::vector<int> Aimbot::GetTargetHitboxes()
{
	std::vector<int> hbx;

	for (int &h : MenuControls::a_hitboxes->GetSelectedItems())
	{
		switch (h)
		{
		case 0: // Head
			hbx.push_back(HITBOX_HEAD); 
			break;
		case 1: // Chest
			hbx.push_back(HITBOX_BELLY); 
			hbx.push_back(HITBOX_LOWER_CHEST);
			hbx.push_back(HITBOX_UPPER_CHEST);
			break;
		case 2: // Arms
			hbx.push_back(HITBOX_LEFT_HAND);
			hbx.push_back(HITBOX_RIGHT_HAND);
			hbx.push_back(HITBOX_LEFT_UPPER_ARM);
			hbx.push_back(HITBOX_RIGHT_UPPER_ARM);
			break;
		case 3: // Legs
			hbx.push_back(HITBOX_LEFT_THIGH); 
			hbx.push_back(HITBOX_RIGHT_THIGH); 
			hbx.push_back(HITBOX_LEFT_FOOT);
			hbx.push_back(HITBOX_RIGHT_FOOT);
			break;
		default:
			break;
		}
	}
	return hbx;
}

bool Aimbot::TargetValid(entity* ent) // If entity is valid
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local && ent)
	{
		if (ent->IsPlayer() && !ent->m_bGunGameImmunity())
		{
			if (ent->m_iTeamNum() != local->m_iTeamNum() || MenuControls::a_ignoreteams->GetState())
			{
				if (local->m_iHealth() > 0 && ent->m_iHealth() > 0)
				{
					if (!local->IsDormant() && !ent->IsDormant())
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Aimbot::FastHitScan(entity* ent, entity* local, bool Can_BackTrack)
{
	bool UseAutoWall = MenuControls::a_autowall->GetState();
	bool UseBackTrack = MenuControls::a_backtrack->GetState();

	if (UseBackTrack && Can_BackTrack)
	{
		AimbotBackTrackVector BackTrack = LC::GetVectorToShoot(ent, local);
		
		if (UseBackTrack && BackTrack.record.ID >= 0 && BackTrack.hitbox.vec.Length() > 0 && BackTrack.hitbox.id > -1)
		{
			return true;
		}
	}

	for (int hitbox_id : GetTargetHitboxes())
	{
		Vector hitbox;
		if (ent->GetHitboxPosition(hitbox_id, hitbox))
		{
			if (ent->IsVisiblePosition(hitbox, local))
			{
				return true;
			}
		}
	}

	// Multipoint scan failed so now we try to autowall

	if (UseAutoWall)
	{
		Vector Body(0, 0, 0);
		if (ent->GetHitboxPosition(Hitboxes::HITBOX_UPPER_CHEST, Body))
		{
			if (Autowall::GetDamage(Body, local, ent) > 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool Aimbot::IsVitalHitbox(int hitbox_id)
{
	for (int hbx : VitalHitboxes())
	{
		if (hitbox_id == hbx)
		{
			return true;
		}
	}

	return false;
}

int Aimbot::GetHitGroup(int hitbox)
{
	switch (hitbox)
	{
	case HITBOX_HEAD:
	case HITBOX_NECK:
		return HITGROUP_HEAD;
		break;
	case HITBOX_PELVIS:
	case HITBOX_BELLY:
	case HITBOX_THORAX:
	case HITBOX_LOWER_CHEST:
	case HITBOX_UPPER_CHEST:
		return HITGROUP_CHEST;
		break;
	case HITBOX_RIGHT_THIGH:
	case HITBOX_RIGHT_FOOT:
	case HITBOX_RIGHT_CALF:
		return HITGROUP_RIGHTLEG;
		break;
	case HITBOX_LEFT_CALF:
	case HITBOX_LEFT_THIGH:
	case HITBOX_LEFT_FOOT:
		return HITGROUP_LEFTLEG;
		break;
	case HITBOX_RIGHT_HAND:
	case HITBOX_RIGHT_UPPER_ARM:
	case HITBOX_RIGHT_FOREARM:
		return HITGROUP_RIGHTARM;
		break;
	case HITBOX_LEFT_HAND:
	case HITBOX_LEFT_UPPER_ARM:
	case HITBOX_LEFT_FOREARM:
		return HITGROUP_LEFTARM;
		break;
	default:
		return HITGROUP_GENERIC;
		break;
	}
	return HITGROUP_GENERIC;
}

bool Aimbot::HitChanceTest(entity* Local, entity* Target, Vector Hitbox, Vector Angles, baseweapon* LocalWeapon) // Based on the source posted by @PhilipKDick on UC
{
	if (LocalWeapon)
	{
		LocalWeapon->UpdateAccuracyPenalty();

		float hits = 0, result = 0;
		float spread_cone = LocalWeapon->GetInaccuracy();

		weaponinfo_t* weapon_info = LocalWeapon->GetCSWpnData();
		
		if (weapon_info)
		{
			Vector forward; MathUtils::AngleVectors(Angles, &forward);
			Ray_t ray; trace_t tr;

			for (float i = 0; i < 50.f; i++)
			{
				Vector random_addition(
					MathUtils::RandomFloat(-(spread_cone / 2), spread_cone / 2),
					MathUtils::RandomFloat(-(spread_cone / 2), spread_cone / 2),
					MathUtils::RandomFloat(-(spread_cone / 2), spread_cone / 2)
				);

				ray.Init(Local->GetEyePosition(), Local->GetEyePosition() + (forward + random_addition) * weapon_info->m_flWeaponRange);
				game_interfaces::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, Target, &tr);

				if (tr.hit_entity)
				{
					hits++;
				}
			}
			result = (hits / 50.f) * 100.f;
		}

		if (MenuControls::a_hitchance->GetValue() <= result || UseBackTrack)
		{
			return true;
		}
	}
	return false;
} 

bool Aimbot::CanShoot(entity* LocalPlayer, baseweapon* LocalWeapon) { // If the player can shoot
	if (!LocalWeapon || LocalWeapon->IsInvalidWeapon() || LocalWeapon->m_iClip1() <= 0) 
	{
		return false;
	}
	float flServerTime = LocalPlayer->m_nTickBase() * game_interfaces::Globals->interval_per_tick; // LOCAL TICK BASE BROKEN TEMP FIX HERE
	if (LocalWeapon->m_flNextPrimaryAttack() <= flServerTime)
	{
		return true;
	}
	return false;
}

int Aimbot::GetTarget(entity* local) // We get a new aimbot target
{
	for (int i = 0; i <= game_interfaces::Globals->maxClients; i++)
	{ 
		if (i != game_interfaces::Engine->GetLocalPlayer())
		{
			entity* ent = game_interfaces::EntityList->GetClientEntity(i);
			if (TargetValid(ent))
			{
				bool hitscan = FastHitScan(ent, local, true);
				if (hitscan)
				{
					return i;// If we found a valid player, we break the loop (stop searching players)
				}
			}
		}
	}
	return -1;
}

void Aimbot::AutoRevolver(bool Aimbot_Shooting, baseweapon* localweapon, CUserCmd* cmd)
{
	if (localweapon)
	{
		if (localweapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER && !Aimbot_Shooting)
		{
			Aimbot::AutoRevolverWorking = true;

			float flPostponeFireReady = localweapon->GetPostponeFireReadyTime();

			if (AutoRevolverStopTimer <= 0)
			{
				cmd->buttons |= IN_ATTACK;
			}
			else {
				if (AutoRevolverStopTimer > 0)
				{
					AutoRevolverStopTimer -= 1;
				}
			}
			if (flPostponeFireReady > 0 && (flPostponeFireReady - 1.f < game_interfaces::Globals->curtime))
			{
				AutoRevolverStopTimer = 20;
				cmd->buttons &= ~IN_ATTACK;
			}
		}
		else {
			Aimbot::AutoRevolverWorking = false;
		}
	}
}

void Aimbot::NoRecoil(Vector weapon_recoil,entity* local, baseweapon* localweapon,CUserCmd* cmd)
{
	if (local && localweapon)
	{
		if (weapon_recoil.Length() > 0)
		{
			Vector calc = weapon_recoil * 2;
			if (calc.IsValid() && calc.Length() > 0)
			{
				cmd->viewangles -= calc; // NoRecoil
			}
		}
	}
}

Vector Aimbot::AutoStop(entity* local, baseweapon* weapon, CUserCmd* cmd)
{
	if (weapon)
	{
		if (weapon->GetCSWpnData())
		{
			Vector velocity = local->m_vecVelocity();

			Vector viewangles;
			game_interfaces::Engine->GetViewAngles(viewangles);

			Vector direction;
			MathUtils::VectorAngles(velocity, direction);
			direction.y = viewangles.y - direction.y;

			Vector negated_direction = direction * -(velocity.Length());

			if (velocity.Length() >= (weapon->GetCSWpnData()->m_flMaxSpeed * .34f))
			{
				return negated_direction;
				/*cmd->forwardmove = negated_direction.x;
				cmd->sidemove = negated_direction.y;*/
			}
		}
	}
	return Vector(0, 0, 0);
}

#include "LoggerUtils.h"

void Aimbot::AutoShoot(entity* local, entity* ent, Vector Result, baseweapon* localweapon, CUserCmd* cmd, bool& SendPacket)
{
	if (localweapon && ent && local && cmd)
	{
		if (CanShoot(local, localweapon)) // If player can shoot
		{
			if (!LastWeaponUsed)
			{
				LastWeaponUsed = localweapon;
			}
				
			if (LastWeaponUsed->m_iItemDefinitionIndex() != localweapon->m_iItemDefinitionIndex())
			{
				LastWeaponUsed = localweapon;
				cmd->buttons &= ~IN_ATTACK;
				return;
			}
			LastWeaponUsed = localweapon;

			Vector old_AbsAngles = ent->GetAbsAngles(), old_AbsOrigin = ent->GetAbsOrigin();

			if (UseBackTrack)
			{
				ent->SetAbsOrigin(BackTrack_Record.position);
				ent->SetAbsAngles(BackTrack_Record.angles);
			}
			cmd->buttons |= IN_ATTACK; // Press the shoot button 

			AimbotShooting = true;

			ent->SetAbsOrigin(old_AbsOrigin);
			ent->SetAbsAngles(old_AbsAngles);
		}
	}
}

void Aimbot::SetAngle(Vector vec, CUserCmd* cmd)
{
	if (!MenuControls::a_silent->GetState())
	{
		game_interfaces::Engine->SetViewAngles(vec); // We set the view
	}

	cmd->viewangles = vec; // Aimbot silent
}

Vector Aimbot::CalculateAngle(AimbotHitbox hitbox, entity* ent, entity* local)
{
	Vector Result, HitBox_Pos = hitbox.vec, Delta; // Delta 

	bool Can_Be_PointScaled = false; // We use this to avoid pointscaling feet or hands because we won't hit

	switch (hitbox.id)
	{
	case HITBOX_HEAD:
	case HITBOX_PELVIS:
	case HITBOX_UPPER_CHEST:
	case HITBOX_NECK:
	case HITBOX_THORAX:
		Can_Be_PointScaled = true;
		break;
	default:
		Can_Be_PointScaled = false;
		break;
	}

	if (!UseBackTrack && Can_Be_PointScaled)
	{
		matrix3x4_t matrix[128];
		if (ent->SetupBones(matrix, 128, 256, GetTickCount64()))
		{
			studiohdr_t *studioHdr = game_interfaces::ModelInfo->GetStudiomodel(ent->GetModel());
			if (studioHdr)
			{
				mstudiohitboxset_t *set = studioHdr->GetHitboxSet(0);
				if (set)
				{
					mstudiobbox_t *hbx = set->GetHitbox(hitbox.id);
					if (hbx)
					{
						Vector max, min;

						float mod = hbx->m_flRadius != -1.f ? hbx->m_flRadius : 0.f;

						MathUtils::VectorTransform(hbx->bbmax + Vector(mod, mod, mod), matrix[hbx->bone], max);
						MathUtils::VectorTransform(hbx->bbmin - Vector(mod, mod, mod), matrix[hbx->bone], min);

						Vector new_center = (min + max) * 0.5f;
						Vector center = hitbox.vec;

						float remove_ = fabs(center.z - new_center.z);

						float total_dist = fabs(min.z - max.z);

						float new_dis = ((float)MenuControls::a_pointscale->GetValue() / 100.f) * total_dist;

						center.z = min.z + (new_dis - remove_);

						HitBox_Pos = center;
					}
				}
			}
		}
	}
	
	Delta = HitBox_Pos - local->GetEyePosition();
	
	MathUtils::VectorAngles(Delta, Result); // We calculate the angle for the aimbot

	MathUtils::AntiUntrusted_ClampAngles(Result); // To avoid untrusted bans

	return Result;
}

void Aimbot::SetTickCount(CUserCmd* cmd, entity* local, entity* ent)
{
	Aimbot::Extrapolating = false; 
	if (UseBackTrack)
	{
		cmd->tick_count = TIME_TO_TICKS(BackTrack_Record.SimulationTime + LC::GetLerpTime()); // We set the backtrack tick
	}
	else {
		cmd->tick_count = TIME_TO_TICKS(ent->extrapolation_simulationtime + LC::GetLerpTime()); // We have to compensate
	}
}

bool Aimbot::IsValidHitbox(AimbotHitbox hbx, entity* local, entity* ent)
{
	float dmg = Autowall::GetDamage(hbx.vec,local,ent);
	if (dmg >= MenuControls::a_mindamage->GetValue())
	{
		return true;
	}
	return false;
}

Vector Aimbot::GetNormalPoint(entity* ent, int id)
{
	Vector hbx;
	Vector result;

	if (ent->GetHitboxPosition(id, hbx))
	{
		matrix3x4_t matrix[128];
		if (ent->SetupBones(matrix, 128, 256, GetTickCount64()))
		{
			studiohdr_t *studioHdr = game_interfaces::ModelInfo->GetStudiomodel(ent->GetModel());
			if (studioHdr)
			{
				mstudiohitboxset_t *set = studioHdr->GetHitboxSet(0);
				if (set)
				{
					mstudiobbox_t *hbx = set->GetHitbox(id);
					if (hbx)
					{
						Vector max, min;

						float rad = hbx->m_flRadius != -1.f ? hbx->m_flRadius : 0.f;

						MathUtils::VectorTransform(hbx->bbmax + Vector(rad, rad, rad), matrix[hbx->bone], max);
						MathUtils::VectorTransform(hbx->bbmin - Vector(rad, rad, rad), matrix[hbx->bone], min);

						result = (min + max) * 0.5f;
					}
				}
			}
		}
	}
	return result;
}

std::vector<AimbotMultiPoint> Aimbot::GetMultiPoints(entity* ent, int id)
{
	std::vector<AimbotMultiPoint> Points;

	Vector hbx;
	if (ent->GetHitboxPosition(id, hbx))
	{
		matrix3x4_t matrix[128];
		if (ent->SetupBones(matrix, 128, 256, GetTickCount64()))
		{
			studiohdr_t *studioHdr = game_interfaces::ModelInfo->GetStudiomodel(ent->GetModel());
			if (studioHdr)
			{
				mstudiohitboxset_t *set = studioHdr->GetHitboxSet(0);
				if (set)
				{
					mstudiobbox_t *hbx = set->GetHitbox(id);
					if (hbx)
					{
						Vector max, min;

						float rad = hbx->m_flRadius != -1.f ? hbx->m_flRadius : 0.f;

						MathUtils::VectorTransform(hbx->bbmax + Vector(rad, rad, rad), matrix[hbx->bone], max);
						MathUtils::VectorTransform(hbx->bbmin - Vector(rad, rad, rad), matrix[hbx->bone], min);

						Vector center = (min + max) * 0.5f;

						int max_points = MenuControls::a_multipoint->GetValue();

						if (rad > 0.f && max_points > 0)
						{
							for (int i = 0; i < max_points; i += 4)
							{
								Vector new_vec = center + Vector(i, 0, 0);
								Points.push_back(AimbotMultiPoint(new_vec, id == HITBOX_HEAD ? false : true));

								new_vec = center - Vector(i, 0, 0);
								Points.push_back(AimbotMultiPoint(new_vec, id == HITBOX_HEAD ? false : true));

								new_vec = center + Vector(0, 0, i);
								Points.push_back(AimbotMultiPoint(new_vec, id == HITBOX_HEAD ? false : true));

								new_vec = center - Vector(0, 0, i);
								Points.push_back(AimbotMultiPoint(new_vec, id == HITBOX_HEAD ? false : true));
							}
						}
						else {
							Points.push_back(AimbotMultiPoint(center, false));
						}
					}
				}
			}
		}
	}
	return Points;
}

void Aimbot::UpdateShotsFired(baseweapon* weapon, CUserCmd* cmd)
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local)
	{
		if (cmd->buttons & IN_ATTACK && CanShoot(local, weapon) || cmd->buttons & IN_ATTACK2 && CanShoot(local, weapon))
		{
			if (Aimbot::LastBullets != weapon->m_iClip1())
			{
				Aimbot::LastBullets = weapon->m_iClip1();
				Aimbot::ShotsFired += 1;
			}
		}
		else {
			Aimbot::ShotsFired = 0;
		}
	}
}


void Aimbot::Aimbot(CUserCmd* cmd, entity* local, baseweapon* localweapon, Vector &MovementVector, bool& SendPacket)
{
	if (MenuControls::a_master_switch->GetState())
	{
		if (local && localweapon)
		{
			if (localweapon->IsInvalidWeapon() || GetAsyncKeyState(0x45)) // Knife, C4 and Grenades | E Pressed
				return;
			

			UpdateShotsFired(localweapon, cmd);

			if (Aimbot::CurrentTarget < 0) // We don't have a target
			{
				Aimbot::CurrentTarget = GetTarget(local); // We search a entity
			}
			else {
				// If we got a target				
				entity* ent = game_interfaces::EntityList->GetClientEntity(Aimbot::CurrentTarget);
				if (TargetValid(ent)) // If the target is valid
				{
					bool HitScan_Succeded = false;
					AimbotHitbox HitScan_Hitbox(0, Vector(0,0,0));
					HitScan_Succeded = Aimbot::ExpensiveHitScan(ent, local, &HitScan_Hitbox);
					if (HitScan_Succeded && HitScan_Hitbox.vec.Length() > 0) // If the hitbox is valid
					{
						SetTickCount(cmd, local, ent);
						
						Vector Result = Aimbot::CalculateAngle(HitScan_Hitbox, ent, local);
						MathUtils::AntiUntrusted_ClampAngles(Result);
						Aimbot::SetAngle(Result, cmd);
				

						if (MenuControls::a_autoscope->GetState() && localweapon->m_bIsScopedWeapon() && !local->m_bIsScoped())
						{
							cmd->buttons |= IN_ATTACK2;
						}
						else {
							if (MenuControls::a_hitchance->GetValue() > 0)
							{
								if (HitChanceTest(local, ent, HitScan_Hitbox.vec, Result, localweapon))
								{
									if (MenuControls::a_shoot->GetState())
									{
										Aimbot::AutoShoot(local, ent, Result, localweapon, cmd, SendPacket);
									}
								}
								else {
									if (MenuControls::a_autostop->GetState())
									{
										Vector autostop = Aimbot::AutoStop(local, localweapon, cmd);
										if (autostop.Length() > 0.f)
										{
											MovementVector = Aimbot::AutoStop(local, localweapon, cmd);
										}
									}
								}
							}
							else {
								if (MenuControls::a_shoot->GetState())
								{
									Aimbot::AutoShoot(local, ent, Result, localweapon, cmd, SendPacket);
								}
							}
						}
					}
					else {
						Aimbot::CurrentTarget = -1;
					}	
				}
				else {
					Aimbot::CurrentTarget = -1;
				}			
			}

			if (MenuControls::a_autorevolver->GetState())
			{
				Aimbot::AutoRevolver(AimbotShooting, localweapon, cmd);
			}
			else {
				Aimbot::AutoRevolverWorking = false;
			}
		}
	}else{
		Aimbot::AutoRevolverWorking = false;
		Aimbot::Extrapolating = false;
	}
}
