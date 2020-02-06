#pragma once

#include "CUserCmd.h"
#include "Entity.h"

enum ManualDirection
{
	MANUAL_DIRECTION_LEFT,
	MANUAL_DIRECTION_RIGHT,
	MANUAL_DIRECTION_NONE,
};

namespace AntiAim
{
	extern ManualDirection Manual_Direction;

	extern Vector AntiAimAngles_Real;
	extern Vector AntiAimAngles_Fake;

	float GetAntiAimYaw(entity* local, int num, int addition);
	float GetAntiAimPitch(int num);

	void UpdateGhosts(CUserCmd* cmd);
	void AtTarget(entity* local, Vector &v);
	void AntiAim(CUserCmd* cmd, entity* local, bool &SendPacket);
	bool FreeStanding(CUserCmd* cmd, entity* local, entity* target);
}