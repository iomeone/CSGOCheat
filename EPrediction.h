#pragma once

#include "CUserCmd.h"
#include "Entity.h"

namespace Prediction
{
	void stop(entity* local, CUserCmd* cmd);
	void start(CUserCmd* cmd, entity* local);
}
