#pragma once

#include "EngineTrace.h"
#include "RandomStuff.h"

class ITraceEngine
{
public:
	int	GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, entity** ppEntity = NULL)
	{
		typedef int(__thiscall* original)(void*, const Vector&, int, entity**);
		return vfunction<original>(this, 0)(this, vecAbsPosition, contentsMask, ppEntity);
	}
	// Traces a ray against a particular entity
	void ClipRayToEntity(const Ray_t &ray, unsigned int fMask, entity *pEnt, trace_t *pTrace)
	{
		typedef void(__thiscall* original)(void*, const Ray_t&, unsigned int, entity*, trace_t*);
		vfunction<original>(this, 3)(this, ray, fMask, pEnt, pTrace);
	}
	// A version that simply accepts a ray (can work as a traceline or tracehull)
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* original)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		vfunction<original>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
};