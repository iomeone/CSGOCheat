#pragma once

#ifndef AIMBOT_H
#define AIMBOT_H

#include "CUserCmd.h"
#include "Entity.h"
#include "LagCompensation.h"
#include "AimbotStruct.h"

namespace NoSpread
{
	void NoSpread(entity* local, baseweapon* localweapon, CUserCmd* cmd);
}



namespace Aimbot
{
	extern int CurrentTarget;
	extern int ShotsFired;
	extern int LastBullets;
	extern bool AutoRevolverWorking;
	extern int AutoRevolverStopTimer;
	extern bool Extrapolating;
	extern bool AimbotShooting;
	extern bool UseBackTrack;
	extern BackTrackRecord BackTrack_Record;

	std::vector<int> VitalHitboxes();
	std::vector<int> GetTargetHitboxes();

	int GetTarget(entity* local);
	int GetHitGroup(int hitbox);

	bool TargetValid(entity* ent);
	bool FastHitScan(entity* ent, entity* local, bool Can_BackTrack);
	bool ExpensiveHitScan(entity* ent, entity* local, AimbotHitbox* HitBox);
	bool HitChanceTest(entity* Local, entity* Target, Vector Hitbox, Vector Angles, baseweapon* LocalWeapon);
	bool CanShoot(entity* LocalPlayer, baseweapon* LocalWeapon);
	bool IsVitalHitbox(int hitbox_id);
	bool IsValidHitbox(AimbotHitbox hbx, entity* local, entity* ent);

	Vector CalculateAngle(AimbotHitbox hitbox, entity* ent, entity* local);
	void SetTickCount(CUserCmd* cmd, entity* local,entity* ent);
	void SetAngle(Vector vec, CUserCmd* cmd);
	Vector AutoStop(entity* local, baseweapon* weapon, CUserCmd* cmd);
	void AutoShoot(entity* local, entity* ent, Vector Result, baseweapon* localweapon, CUserCmd* cmd, bool& SendPacket);
	void AutoRevolver(bool Aimbot_Shooting, baseweapon* localweapon, CUserCmd* cmd);
	void Aimbot(CUserCmd* cmd, entity* local, baseweapon* localweapon, Vector &MovementVector, bool& SendPacket);
	void NoRecoil(Vector weapon_recoil, entity* local, baseweapon* localweapon, CUserCmd* cmd);
	void UpdateShotsFired(baseweapon* weapon, CUserCmd* cmd);
	std::vector<AimbotMultiPoint> GetMultiPoints(entity* ent, int id);
	Vector GetNormalPoint(entity* ent, int id);
}
#endif