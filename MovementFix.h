#pragma once

#include "CUserCmd.h"

namespace MovementFix
{
	void start(CUserCmd* cmd);
	void end(CUserCmd* cmd, Vector movementvector);
}