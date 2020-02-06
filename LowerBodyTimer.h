#pragma once

#include "Entity.h"

namespace LBYTimer
{
	extern float old_curtime;
	extern float hold_time;

	extern bool lby_updated;
	extern bool was_moving;
	extern bool lby_can_break;


	void Update();
}