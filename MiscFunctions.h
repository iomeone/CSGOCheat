#pragma once

#include "CUserCmd.h"
#include "Entity.h"

namespace FakeLag
{
	extern int RandomFakeLagValue;
	extern Vector LastChokePosition;
	extern Vector LastChokeAngles;

	void FakeLag(entity* local, bool& SendPacket);
}

namespace Misc_Functions
{
	extern CUserCmd* LastUserCmd;
	extern bool ThirdPersonSwitch;

	void ThirdPerson();
	void FakeWalk(CUserCmd* cmd, bool& SendPacket);
	void SetClantag(const char* tag);
	void UpdateClanTag();
	void UpdatePrediction(entity* local);
	void UpdateGroundPitch(entity* local);
	void UpdateNightMode();

	void CircleStrafeDraw(entity* local);
	Vector CircleStrafe(CUserCmd* cmd, entity* local, Vector angles);
}