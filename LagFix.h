#pragma once

#include "Vector.h"
#include <vector>
#include "Entity.h"

struct lag_data
{
	Vector m_vecVelocity;
	Vector m_vecSimulatedOrigin;
	float SimulatedTime;
	int Flags;
};

namespace LagFix
{
	extern lag_data lagdata[128];
	extern lag_data old_lagdata[128];
	void Update();
	void SimulateMovement(lag_data &data, bool in_air, entity* pBaseEntity);
}