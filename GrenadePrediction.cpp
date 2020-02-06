#include "RandomStuff.h"
#include "InterfaceManager.h"
#include "GrenadePrediction.h"
#include "MathUtils.h"
#include "RenderUtils.h"
#include "Menu.h"
#include "AutoWall.h"

// Credits to @n0xius & @Casual_Hacker on UC

ConVar* molotov_throw_detonate_time;
ConVar* weapon_molotov_maxdetonateslope;
ConVar* gravity;

float GetGrenadeDetonateTime(int item)
{
	switch (item)
	{
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		return 1.5f;
		break;
	case WEAPON_INCGRENADE:
	case WEAPON_MOLOTOV:
		return molotov_throw_detonate_time->GetFloat();
		break;
	case WEAPON_TAGRENADE:
		return 5.f;
		break;
	}

	return 3.f;
};

int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	int blocked = 0;
	float angle = normal[2];

	if (angle > 0)
		blocked |= 1; // floor

	if (!angle)
		blocked |= 2; // step

	float backoff = in.Dot(normal) * overbounce;

	for (int i = 0; i < 3; i++)
	{
		out[i] = in[i] - (normal[i] * backoff);

		if (out[i] > -0.1f && out[i] < 0.1f)
			out[i] = 0;
	}

	return blocked;
};

void Line(Vector v, Vector v2)
{
	Vector W2S, W2S_2;
	if (RenderUtils::WorldToScreen(v, W2S) && RenderUtils::WorldToScreen(v2, W2S_2))
	{
		RenderUtils::RenderLine(W2S.x, W2S.y, W2S_2.x, W2S_2.y, MenuControls::v_other_grenade_prediction->GetColor());
	}
}

void GrenadePred::GrenadePrediction(entity* localPlayer, baseweapon* weapon)
{
	if (!molotov_throw_detonate_time)
	{
		molotov_throw_detonate_time = game_interfaces::CVar->FindVar("molotov_throw_detonate_time");
	}
	if (!weapon_molotov_maxdetonateslope)
	{
		weapon_molotov_maxdetonateslope = game_interfaces::CVar->FindVar("weapon_molotov_maxdetonateslope");
	}
	if (!gravity)
	{
		gravity = game_interfaces::CVar->FindVar("sv_gravity");
	}

	playerinfo playerInfo;
	auto itemIndex = weapon->m_iItemDefinitionIndex();
	auto wpnData = weapon->GetCSWpnData();

	if (!wpnData || weapon->GetWeaponType() != WEAPON_TYPE_GRENADE || !weapon->m_bPinPulled())
		return;

	Vector angThrow, forward;

	game_interfaces::Engine->GetViewAngles(angThrow);

	MathUtils::AntiUntrusted_ClampAngles(angThrow);
	angThrow.x -= (90.f - abs(angThrow.x)) * 0.11111111f;

	MathUtils::AngleVectors(angThrow, &forward);

	float throwStrengh = weapon->m_flThrowStrength();
	float throwVelocity = fmin(fmax(wpnData->m_flThrowVelocity * 0.9f, 15.f), 750.f);

	float throwHeight = (throwStrengh * 12.f) - 12.f;
	float v68 = throwVelocity * ((0.7f * throwStrengh) + 0.3f);

	Vector startPos = localPlayer->GetEyePosition() + Vector(0, 0, throwHeight), endPos = startPos + (forward * 22.f);

	CTraceFilter filter;
	filter.pSkip = localPlayer;

	trace_t trace;
	Ray_t ray;
	ray.Init(startPos, endPos);
	game_interfaces::Trace->TraceRay(ray, MASK_SOLID | CONTENTS_CURRENT_90, &filter, &trace);

	if (trace.contents & CONTENTS_WINDOW)
	{
		CTraceFilterEntitiesOnly new_filter;
		new_filter.pEnt = localPlayer;
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID | CONTENTS_CURRENT_90, &new_filter, &trace);
		endPos = trace.endpos;
	}
	else {
		endPos = trace.endpos - (forward * 6.f);
	}

	auto throwPos = (localPlayer->m_vecVelocity() * 1.25f) + (forward * v68);

	float gravity_float = (gravity->GetFloat() * 0.4f);

	for (int ticks = TIME_TO_TICKS(GetGrenadeDetonateTime(itemIndex)); ticks >= 0; --ticks)
	{
		auto throwDir = Vector(throwPos.x, throwPos.y, (throwPos.z + (throwPos.z - (gravity_float * game_interfaces::Globals->interval_per_tick))) * 0.5f);

		auto temp = throwDir * game_interfaces::Globals->interval_per_tick;

		throwPos.z -= gravity_float * game_interfaces::Globals->interval_per_tick;

		auto src = endPos, end = endPos + temp;

		ray.Init(src, end);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID | CONTENTS_CURRENT_90, &filter, &trace);

		if (trace.allsolid)
			throwPos = Vector(0, 0, 0);

		if (trace.contents & CONTENTS_WINDOW)
		{
			CTraceFilterEntitiesOnly new_filter;
			new_filter.pEnt = localPlayer;
			game_interfaces::Trace->TraceRay(ray, MASK_SOLID | CONTENTS_CURRENT_90, &new_filter, &trace);
			endPos = trace.endpos;
		}
		else {
			endPos = trace.endpos;
		}

		Line(src, endPos);

		if (trace.fraction != 1.f)
		{
			float surfaceElasticity = 1.f;

			Vector throwPos2;

			PhysicsClipVelocity(throwPos, trace.plane.normal, throwPos2, 2.f);

			if (trace.hit_entity && game_interfaces::Engine->GetPlayerInfo(trace.hit_entity->GetIndex(), &playerInfo))
				surfaceElasticity = 0.3f;

			throwPos2 *= clamp(surfaceElasticity * 0.45f, 0.f, 0.9f);

			end = endPos + (throwPos2 * ((1.f - trace.fraction) * game_interfaces::Globals->interval_per_tick));

			// NOTE: the molotov and incendiary grenade only bounce of walls
			if (itemIndex == WEAPON_MOLOTOV
				|| itemIndex == WEAPON_INCGRENADE)
			{
				if (trace.plane.normal.z >= cos(DEG2RAD(weapon_molotov_maxdetonateslope->GetFloat())))
					return;
			}

			// NOTE: the tactical awareness grenade sticks to surfaces
			if (itemIndex == WEAPON_TAGRENADE)
				return;

			ray.Init(endPos, end);
			game_interfaces::Trace->TraceRay(ray, MASK_SOLID | CONTENTS_CURRENT_90, &filter, &trace);
			
			Line(endPos, end);

			endPos = trace.endpos;
			throwPos = throwPos2;
		}
	}
}