#include "AntiAim.h"
#include "Aimbot.h"
#include "AutoWall.h"
#include "Menu.h"
#include "LowerBodyTimer.h"
#include <random>

#include "InterfaceManager.h"

ManualDirection AntiAim::Manual_Direction = MANUAL_DIRECTION_NONE;
Vector AntiAim::AntiAimAngles_Real = Vector(0, 0, 0);
Vector AntiAim::AntiAimAngles_Fake = Vector(0, 0, 0);

int LadderTimer = 0;

bool CouldShoot(entity* LocalPlayer, baseweapon* LocalWeapon) { // If the player can shoot
	if (!LocalWeapon)
		return false;
	if (!LocalWeapon->GetCSWpnData())
		return false;
	if (LocalWeapon->m_iClip1() <= 0 && LocalWeapon->GetWeaponType() != WEAPON_TYPE_KNIFE)
		return false;

	float flServerTime = LocalPlayer->GetCorrectedTickBase() * game_interfaces::Globals->interval_per_tick;

	if (LocalWeapon->m_flNextPrimaryAttack() <= flServerTime)
	{
		return true;
	}

	return true;
}

bool ShouldAntiAimReturn(entity* local, CUserCmd* cmd)
{
	baseweapon* LocalWeapon = (baseweapon*)local->GetWeapon();
	if (LocalWeapon)
	{
		weaponinfo_t* weapon_info = LocalWeapon->GetCSWpnData();
		if (weapon_info)
		{
			// If a grenade is being thrown
			if (LocalWeapon->GetWeaponType() == WEAPON_TYPE_GRENADE && !LocalWeapon->m_bPinPulled() && !CouldShoot(local, LocalWeapon))
			{
				return true;
			}
			if (CouldShoot(local, LocalWeapon) && (cmd->buttons & IN_ATTACK))
			{
				if (MenuControls::a_autorevolver->GetState() && LocalWeapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
				{
					if (LocalWeapon->GetPostponeFireReadyTime() > 0 && !Aimbot::AutoRevolverWorking)
					{
						return true;
					}
				}
				else {
					return true;
				}
			}
			if (CouldShoot(local, LocalWeapon) && (cmd->buttons & IN_ATTACK2))
			{
				if (LocalWeapon->GetWeaponType() == WEAPON_TYPE_KNIFE)
				{
					return true;
				}
			}
		}
	}

	// If the key e is pressed or the movement is invalid
	if (GetAsyncKeyState(0x45) || LadderTimer > 0 || local->m_moveType() == MOVETYPE_NOCLIP /*(*game_interfaces::PlayerResources)->m_bFreezePeriod()*/)
	{
		return true;
	}

	return false;
}


void SetAA_AngleX(CUserCmd* cmd, float angle)
{
	Vector clamp_vec = Vector(angle, 0, 0);
	MathUtils::AntiUntrusted_ClampAngles(clamp_vec);
	cmd->viewangles.x = clamp_vec.x;
}
void SetAA_AngleY(CUserCmd* cmd, float angle)
{
	Vector clamp_vec = Vector(0, angle, 0);
	MathUtils::AntiUntrusted_ClampAngles(clamp_vec);
	cmd->viewangles.y = clamp_vec.y;
}

void AntiAim::UpdateGhosts(CUserCmd* cmd)
{
	MathUtils::AntiUntrusted_ClampAngles(cmd->viewangles);
	AntiAim::AntiAimAngles_Real.x = cmd->viewangles.x;
	AntiAim::AntiAimAngles_Real.y = cmd->viewangles.y;
	AntiAim::AntiAimAngles_Real.z = 0.f;

}

float wall_thickness(entity* local, entity* target, Vector target_head, Vector local_head) // Based on the code posted by @wep on UC
{
	Ray_t ray;
	trace_t trace, trace2;
	CTraceFilterSkipTwoEntities filter(local, target);

	ray.Init(local_head, target_head);
	game_interfaces::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace);

	ray.Init(target_head, local_head);
	game_interfaces::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

	if (trace.DidHit() && trace2.DidHit())
	{
		return trace.endpos.DistTo(trace2.endpos);
	}
	return -1.f;
}

bool AntiAim::FreeStanding(CUserCmd* cmd, entity* local, entity* target) // Based on the code posted by @wep on UC
{

	Vector head;
	if (local->GetHitboxPosition(HITBOX_PELVIS, head))
	{
		float thickest = -1.f;
		float best_rotation = 0.f;

		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += ((2 * M_PI) / 8.f))
		{
			Vector newhead(
				fabs(Vector(head - local->m_vecOrigin()).Length2D()) * cos(rotation) + local->GetEyePosition().x,
				fabs(Vector(head - local->m_vecOrigin()).Length2D()) * sin(rotation) + local->GetEyePosition().y,
				local->GetEyePosition().z
			);

			float wall_thickness_result = wall_thickness(target, local, target->GetAbsOrigin(), newhead);

			if (wall_thickness_result > thickest)
			{
				thickest = wall_thickness_result;
				best_rotation = rotation;
			}
		}

		if (thickest > -1.f)
		{
			cmd->viewangles.y = RAD2DEG(best_rotation);
			return true;
		}
	}

	return false;
}

bool Freestanding(CUserCmd* cmd, entity* local)
{
	if (MenuControls::a_antiaim_stand_freestanding->GetState())
	{
		int TargetFound = -1;
		float shortdistance = 99999999.f;

		for (int i = 0; i <= game_interfaces::Globals->maxClients; i++) // Player loop
		{
			if (i != game_interfaces::Engine->GetLocalPlayer())
			{
				entity* ent = game_interfaces::EntityList->GetClientEntity(i);
				if (ent)
				{
					if (ent->m_iHealth() > 0 && !ent->IsDormant()
						&& ent->m_iTeamNum() != local->m_iTeamNum())
					{
						float distance = ent->m_vecOrigin().DistToSqr(local->m_vecOrigin());
						if (distance < shortdistance)
						{
							shortdistance = shortdistance;
							TargetFound = i;
						}
					}
				}
			}
		}
		if (TargetFound >= 0)
		{
			entity* target = game_interfaces::EntityList->GetClientEntity(TargetFound);
			return AntiAim::FreeStanding(cmd, local, target);
		}
	}
	return false;
}

void ManualKeys()
{
	if (MenuControls::a_antiaim_manual->GetState())
	{
		if (GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_DOWN))
		{
			AntiAim::Manual_Direction = MANUAL_DIRECTION_LEFT;
		}
		else {
			if (GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_DOWN))
			{
				AntiAim::Manual_Direction = MANUAL_DIRECTION_RIGHT;
			}
			else {
				if (!GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
				{
					AntiAim::Manual_Direction = MANUAL_DIRECTION_NONE;
				}
			}
		}
	}
}

void AntiAim::AtTarget(entity* local, Vector &v)
{
	int TargetFound = -1;
	float shortdistance = 99999999.f;

	for (int i = 0; i <= game_interfaces::Globals->maxClients; i++)
	{
		if (i != game_interfaces::Engine->GetLocalPlayer())
		{
			entity* ent = game_interfaces::EntityList->GetClientEntity(i);
			if (ent)
			{
				if (ent->m_iHealth() > 0 && !ent->IsDormant() && ent->m_iTeamNum() != local->m_iTeamNum())
				{
					float distance = ent->m_vecOrigin().DistToSqr(local->m_vecOrigin());
					if (distance < shortdistance)
					{
						shortdistance = shortdistance;
						TargetFound = i;
					}
				}
			}
		}
	}
	if (TargetFound >= 0)
	{
		entity* target = game_interfaces::EntityList->GetClientEntity(TargetFound);
		if (target)
		{
			Vector HitBox;
			if (target->GetHitboxPosition(HITBOX_HEAD, HitBox))
			{
				Vector Delta = HitBox - local->GetEyePosition();

				Vector ca;

				MathUtils::VectorAngles(Delta, ca);

				v = ca;
			}
		}
	}
}

int Spin;
int ProgressiveValue;

#include "LoggerUtils.h"

float AntiAim::GetAntiAimYaw(entity* local, int num, int addition)
{
	Vector Real;
	game_interfaces::Engine->GetViewAngles(Real);

	if (MenuControls::a_antiaim_at_target->GetState())
	{
		AntiAim::AtTarget(local, Real);
	}

	MathUtils::AntiUntrusted_ClampAngles(Real);

	float new_ang = 0.f;
	switch (num)
	{
	case 0:
		// Off
		break;
	case 1:
		// Jitter
		new_ang = MathUtils::GetRandom(-45, 45);
		new_ang += addition;
		return new_ang;
		break;
	case 2:
		// Spin
		if (Spin >= 179)
		{
			Spin = -179;
		}
		else {
			Spin += 6;
		}
		return Spin + addition;
		break;
	case 3:
		// Sideways
		return (Real.y - 90) + addition;
		break;
	case 4:
		// Random
		new_ang = MathUtils::GetRandom(-180, 180);
		new_ang += addition;
		return new_ang;
		break;
	case 5:
		// Static
		new_ang = addition;
		return new_ang;
		break;
	case 6:
		// LowerBody
		new_ang = local->m_flLowerBodyYawTarget();
		new_ang += addition;
		return new_ang;
		break;
	case 7:
		// BackWards
		new_ang = Real.y - 180.f;
		new_ang += addition;
		return new_ang;
		break;
	case 8:
		// Progressive
		float Value = ProgressiveValue + addition;
		return Value;
		break;
	}
	return 0.f;
}

float AntiAim::GetAntiAimPitch(int num)
{
	switch (num)
	{
	case 0: //NONE
		break;
	case 1: //DOWN
		return 89.f;
		break;
	case 2: //ZERO
		return 0.f;
		break;
	case 3: //UP
		return -89.f;
		break;
	default:
		break;
	}
	return 0.f;
}

int RealValue = 0;
int FakeValue = 0;
int Timer = 0;

int LBYTimer_FakeAnim = 0;
float Last_Body_Yaw = 0.f;

bool Progressive = true;

float GetInvertedMaxRotation(entity* ent)
{
	animationstate* animstate = ent->GetAnimationState();

	float speedfraction = animstate->m_flFeetSpeedForwardsOrSideWays;

	if (speedfraction > 1.f)
	{
		speedfraction = 1.f;
	}

	float unk1 = (float)((float)(animstate->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * speedfraction;
	float duckamount = animstate->m_fDuckAmount;
	float unk2 = unk1 + 1.0;
	if (duckamount > 0.0)
	{
		float unk = animstate->m_flFeetSpeedUnknownForwardOrSideways;
		if (unk > 1.f)
		{
			unk = 1.f;
		}
		unk2 = unk2 + (float)((duckamount * unk) * (float)(0.5 - unk2));
	}
	float InvertedMaxRotation = *(float *)((uintptr_t)animstate + 0x330) * unk2;

	return InvertedMaxRotation;
}

float GetMaxRotation(entity* ent)
{
	animationstate* animstate = ent->GetAnimationState();

	float speedfraction = animstate->m_flFeetSpeedForwardsOrSideWays;
	
	if (speedfraction > 1.f)
	{
		speedfraction = 1.f;
	}

	float unk1 = (float)((float)(animstate->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * speedfraction;
	float duckamount = animstate->m_fDuckAmount;
	float unk2 = unk1 + 1.0;
	if (duckamount > 0.0)
	{
		float unk = animstate->m_flFeetSpeedUnknownForwardOrSideways;
		if (unk > 1.f)
		{
			unk = 1.f;
		}
		unk2 = unk2 + (float)((duckamount * unk) * (float)(0.5 - unk2));
	}
	float MaxRotation = *(float *)((uintptr_t)animstate + 0x334) * unk2;

	return MaxRotation;
}

float GetGoalFeetYaw(entity* ent)
{
	animationstate* animstate = ent->GetAnimationState();

	float v40 = -360.0;
	float v41 = animstate->m_flGoalFeetYaw;
	float v128 = -360.0;
	if (v41 >= -360.0)
	{
		v40 = fminf(v41, 360.0);
		v128 = v40;
	}
	float player_yaw = animstate->m_flEyeYaw - v40;
	if (animstate->m_flEyeYaw <= v128)
	{
		if (player_yaw <= -180.0)
			player_yaw = player_yaw + 360.0;
	}
	else if (player_yaw >= 180.0)
	{
		player_yaw = player_yaw - 360.0;
	}


	float GoalFeetYaw = animstate->m_flGoalFeetYaw;

	if (player_yaw <= GetMaxRotation(ent))
	{
		if (GetInvertedMaxRotation(ent) > player_yaw)
			GoalFeetYaw = GetInvertedMaxRotation(ent) + animstate->m_flEyeYaw;
	}
	else
	{
		GoalFeetYaw = animstate->m_flEyeYaw - GetInvertedMaxRotation(ent);
	}
	return GoalFeetYaw;
}

void AntiAim::AntiAim(CUserCmd* cmd, entity* local, bool &SendPacket)
{
	if (!MenuControls::a_antiaim->GetState())
		return;

	if (local->m_moveType() == MOVETYPE_LADDER)
	{
		LadderTimer = 10;
	}
	else {
		if (LadderTimer > 0)
		{
			LadderTimer -= 1;
		}
	}

	Vector Real;
	game_interfaces::Engine->GetViewAngles(Real);

	if (MenuControls::a_antiaim_at_target->GetState())
	{
		AntiAim::AtTarget(local, Real);
	}

	MathUtils::AntiUntrusted_ClampAngles(Real);

	if (ShouldAntiAimReturn(local, cmd)) // E Pressed | Aimbot Shooting | Invalid Movement Type
	{
		baseweapon* LocalWeapon = (baseweapon*)local->GetWeapon();
		if (LocalWeapon)
		{
			float flServerTime = local->GetCorrectedTickBase() * game_interfaces::Globals->interval_per_tick;
			float diff = LocalWeapon->m_flNextPrimaryAttack() - flServerTime;
			if (LocalWeapon->m_flNextPrimaryAttack() > flServerTime && (cmd->buttons & IN_ATTACK) && diff < 3 && LocalWeapon->GetWeaponType() != WEAPON_TYPE_KNIFE)
			{
				SendPacket = false;
			}
			else {
				if (diff < 0.1f && LocalWeapon->GetWeaponType() == WEAPON_TYPE_KNIFE && (cmd->buttons & IN_ATTACK))
				{
					SendPacket = false;
				}
			}
		}	
		return;
	}

	if (ProgressiveValue >= 180.f)
	{
		Progressive = false;
	}
	if (ProgressiveValue <= 0.f)
	{
		Progressive = true;
	}
	ProgressiveValue = Progressive ? (ProgressiveValue + 5) : (ProgressiveValue - 5);

	ManualKeys();

	AntiAim::AntiAimAngles_Real.z = 0.f;
	AntiAim::AntiAimAngles_Fake.z = 0.f;

	float AntiAim_Next_Pitch = cmd->viewangles.x;
	float AntiAim_Next_Yaw = cmd->viewangles.y;

	if (local->m_fFlags() & FL_ONGROUND) // On Ground
	{
		if (local->m_vecVelocity().Length2D() > 1.f) // Moving
		{
			AntiAim_Next_Pitch = GetAntiAimPitch(MenuControls::a_antiaim_ground_pitch->GetSelectedItem());
			AntiAim_Next_Yaw = GetAntiAimYaw(local, MenuControls::a_antiaim_ground_yaw->GetSelectedItem(), MenuControls::a_antiaim_ground_yaw_addition->GetValue());
		}
		else { // Not Moving
			AntiAim_Next_Pitch = GetAntiAimPitch(MenuControls::a_antiaim_stand_pitch->GetSelectedItem());
			AntiAim_Next_Yaw = GetAntiAimYaw(local, MenuControls::a_antiaim_stand_yaw->GetSelectedItem(), MenuControls::a_antiaim_stand_yaw_addition->GetValue());
		}
	}
	else { // In Air
		AntiAim_Next_Pitch = GetAntiAimPitch(MenuControls::a_antiaim_air_pitch->GetSelectedItem());
		AntiAim_Next_Yaw = GetAntiAimYaw(local, MenuControls::a_antiaim_air_yaw->GetSelectedItem(), MenuControls::a_antiaim_air_yaw_addition->GetValue());
	}

	if (MenuControls::a_antiaim_anti_lby_air->GetState() && local->GetAnimationState()->m_bInHitGroundAnimation)
	{
		SetAA_AngleY(cmd, local->m_flLowerBodyYawTarget() + 120.f);
	}
	else {
		bool CanUseFreestanding = Freestanding(cmd, local);
		if (!CanUseFreestanding)
		{
			if (MenuControls::a_antiaim_manual->GetState() && MenuControls::a_antiaim_desync->GetState())
			{
				float manual_yaw = 0.f;
				std::cout << Real.y << std::endl;
				switch (AntiAim::Manual_Direction)
				{
				case MANUAL_DIRECTION_LEFT:
					Real.y -= 180.f;
					manual_yaw = Real.y - 10.f;
					break;
				case MANUAL_DIRECTION_RIGHT:
					Real.y += 180.f;
					manual_yaw = Real.y - 10.f;
					break;
				case MANUAL_DIRECTION_NONE:
					Real.y -= 180.f;
					manual_yaw = Real.y;
					break;
				}
				float aa = 0.f;
				if (LBYTimer_FakeAnim < 15)
				{
					SendPacket = false;

					if (AntiAim::Manual_Direction == MANUAL_DIRECTION_RIGHT)
					{
						aa = Progressive ? manual_yaw : manual_yaw + GetMaxRotation(local);
					}
					else {
						if (AntiAim::Manual_Direction == MANUAL_DIRECTION_LEFT)
						{
							aa = Progressive ? manual_yaw : manual_yaw - GetMaxRotation(local);
						}
						else {
							aa = Real.y;
						}
					}

					LBYTimer_FakeAnim += 1;
				}
				else {
					LBYTimer_FakeAnim = 0;
					SendPacket = true;
				}

				AntiAim_Next_Yaw = aa;
			
				Progressive = !Progressive;
				
			}
			else {
				if (MenuControls::a_antiaim_manual->GetState())
				{
					switch (AntiAim::Manual_Direction)
					{
					case MANUAL_DIRECTION_LEFT:
						AntiAim_Next_Yaw = Real.y + 90.f;
						break;
					case MANUAL_DIRECTION_RIGHT:
						AntiAim_Next_Yaw = Real.y - 90.f;
						break;
					case MANUAL_DIRECTION_NONE:
						break;
					}
				}
			}
			SetAA_AngleX(cmd, AntiAim_Next_Pitch);
			SetAA_AngleY(cmd, AntiAim_Next_Yaw);
		}

		if (local->m_vecVelocity().Length2D() < 0.5f)
		{
			switch (MenuControls::a_antiaim_stand_break_mode->GetSelectedItem())
			{
			case 0: // Disabled
				break;
			case 1: // Real Anim
				if (LBYTimer::lby_can_break)
				{
					SetAA_AngleY(cmd, cmd->viewangles.y + MenuControls::a_antiaim_stand_break_value->GetValue());
				}
				break;
			case 2: // Fake Anim
				if (LBYTimer_FakeAnim > 10)
				{
					SetAA_AngleY(cmd, cmd->viewangles.y + MenuControls::a_antiaim_stand_break_value->GetValue());
					LBYTimer_FakeAnim = 0;
				}
				else {
					LBYTimer_FakeAnim += 1;
				}
				break;
			default:
				break;
			}
		}
	}
}