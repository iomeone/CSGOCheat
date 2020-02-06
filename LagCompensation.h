#pragma once

#ifndef LAGCOMPENSATION_H
#define LAGCOMPENSATION_H

#include <vector>

#include "RandomStuff.h"
#include "Entity.h"
#include "Color.h"

#include "NetChannelInfo.h"
#include "CUserCmd.h"
#include "ConVar.h"

#include "AimbotStruct.h"

struct BackTrackRecord
{
	BackTrackRecord(int id)
	{
		this->ID = id;
	}
	BackTrackRecord(){}

	int ID;

	float LowerBody;
	float SimulationTime;
	float Velocity;

	std::vector<AimbotHitbox> Hitboxes;
	std::vector<AimbotHitbox> AutoWallHitboxes;
	
	Vector head_vector;
	Vector position;
	Vector angles;
};

struct BackTrackPlayer
{
	BackTrackPlayer(int index)
	{
		PlayerIndex = index;
		BackTrackMode = 0;
	}

	BackTrackRecord BestRecord;

	Vector LastOrigin;

	int LatestAddedRecord, PlayerIndex, BackTrackMode;

	std::vector<BackTrackRecord> Records;
};

struct AimbotBackTrackVector
{
	AimbotBackTrackVector(AimbotHitbox hitbox, BackTrackRecord r, int mode)
	{
		this->hitbox = hitbox;
		this->record = r;
	}

	AimbotHitbox hitbox;
	BackTrackRecord record;
};

namespace LC
{
	extern std::vector<BackTrackPlayer> BackTrack_Players;
	extern float MaxTicks;

	float GetPing();
	float GetLerpTime();

	bool TickIsValid(float simulation_time);
	bool TargetValid(entity* ent);
	bool IsVisiblePoint(entity* Local, Vector Point);

	void UpdateRecord(BackTrackRecord &Record, entity* ent, entity* local);
	void SetPlayer(entity* ent, BackTrackRecord record);

	BackTrackRecord GetBestRecord(BackTrackPlayer& BackTrackPlayer, entity* ent);
	AimbotBackTrackVector GetVectorToShoot(entity* ent, entity* local);
	AimbotHitbox GetHittableBackTrackPosition(BackTrackRecord Record, entity* local);

	void UpdateBackTrackEntity(entity* ent, entity* local);
	void UpdateBackTrack(entity* local, entity* ent);
	void DrawBackTrack(entity* ent, entity* local);
}
#endif