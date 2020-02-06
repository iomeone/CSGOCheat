#include "HookManager.h"

#include "Trace.h"
#include "InterfaceManager.h"
#include "Menu.h"
#include "Aimbot.h"
#include "AutoWall.h"

TraceRayFn hooks::oTraceRay;

void __fastcall hooks::TraceRay_Hook(void* thisptr, void* ecx, const Ray_t& ray, unsigned int mask, ITraceFilter* trace_filter, trace_t* trace)
{
	hooks::oTraceRay(thisptr, ray, mask, trace_filter, trace);

	/*if (Autowall::UseTraceRayHook && game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		trace->surface.flags |= SURF_SKY;
	}*/
}