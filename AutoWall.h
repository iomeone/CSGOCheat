#pragma once

#include "Entity.h"
#include "EngineTrace.h"

struct FireBulletData
{
	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

namespace Autowall
{
	extern bool UseTraceRayHook;

	void ScaleDamage(int Hitgroup, entity*player, float weapon_armor_ratio, float &current_damage);
	float GetDamage(const Vector& point, entity* local, entity* ent);
	float CanWallbang(Vector point, entity* local, entity* ent);
};

