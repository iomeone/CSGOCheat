#include "LagFix.h"
#include "InterfaceManager.h"
#include "LagCompensation.h"
#include "Menu.h"

ConVar* sv_gravity;
lag_data LagFix::lagdata[128];
lag_data LagFix::old_lagdata[128];

void LagFix::SimulateMovement(lag_data &data, bool in_air, entity* ent)
{
	trace_t trace;
	CTraceFilter filter;
	filter.pSkip = ent;

	int sv_gravity = game_interfaces::CVar->FindVar("sv_gravity")->GetInt();
	float sv_jump_impulse = game_interfaces::CVar->FindVar("sv_jump_impulse")->GetFloat(); // math.sqrt(91200) = 301.1
	auto gravity_per_tick = sv_gravity * game_interfaces::Globals->interval_per_tick;
	auto predicted_origin = data.m_vecSimulatedOrigin;

	if (!in_air)
		data.m_vecVelocity.z -= gravity_per_tick;

	predicted_origin += data.m_vecVelocity * game_interfaces::Globals->interval_per_tick;

	Ray_t ray;
	ray.Init(data.m_vecSimulatedOrigin, predicted_origin, ent->GetCollideable()->OBBMins(), ent->GetCollideable()->OBBMaxs());
	game_interfaces::Trace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

	if (trace.fraction == 1.f)
		data.m_vecSimulatedOrigin = predicted_origin;

	ray.Init(data.m_vecSimulatedOrigin, data.m_vecSimulatedOrigin - Vector(0.f, 0.f, 2.f), ent->GetCollideable()->OBBMins(), ent->GetCollideable()->OBBMaxs());
	game_interfaces::Trace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

	data.Flags &= ~FL_ONGROUND;

	if (trace.fraction == 0.f)
		data.Flags |= FL_ONGROUND;
}

void LagFix::Update()
{
	if (!sv_gravity)
	{
		sv_gravity = game_interfaces::CVar->FindVar("sv_gravity");
	}

	for (int i = 0; i <= game_interfaces::Globals->maxClients; i++)
	{
		if (i != game_interfaces::Engine->GetLocalPlayer())
		{
			entity* ent = game_interfaces::EntityList->GetClientEntity(i);
			if (ent)
			{
				if (!ent->IsDormant() && ent->m_iHealth() > 0)
				{
					lag_data &data = LagFix::lagdata[i];
					lag_data &olddata = LagFix::old_lagdata[i];

					data.m_vecVelocity = ent->m_vecVelocity();
					data.Flags = ent->m_fFlags();
					data.m_vecSimulatedOrigin = ent->GetAbsOrigin();

					animationlayer anim_layer_backup[15];
					for (int i = 0; i < ent->GetNumAnimOverlays(); i++)
					{
						anim_layer_backup[i] = ent->GetAnimOverlay(i);
					}

					INetChannelInfo *nci = game_interfaces::Engine->GetNetChannelInfo();

					float flTickInterval = 1.f / game_interfaces::Globals->interval_per_tick;

					int iTickCount = TIME_TO_TICKS(game_interfaces::Globals->curtime);

					float flDeltaTime = ((floorf(((nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING)) * flTickInterval) + 0.5)
						+ iTickCount
						+ 1)
						* game_interfaces::Globals->interval_per_tick)
						- ent->m_flSimulationTime();

					if (flDeltaTime > 1.f)
						flDeltaTime = 1.f;

					float velocityDegree = RAD2DEG(atan2(data.m_vecVelocity.y, data.m_vecVelocity.x));
					int deltaTicks = floorf((flTickInterval * flDeltaTime) + 0.5);
					float velocityAngle = velocityDegree - RAD2DEG(atan2(olddata.m_vecVelocity.y, olddata.m_vecVelocity.x));

					if (velocityAngle <= 180.0)
					{
						if (velocityAngle < -180.0)
							velocityAngle = velocityAngle + 360.0;
					}
					else
					{
						velocityAngle = velocityAngle - 360.0;
					}

					//flNewVelocityAngle = Utils::NormalizeYaw(flNewVelocityAngle);
					float m_flSimTimeDelta = ent->m_flSimulationTime() - ent->GetOldSimulationTime();
					// How many ticks the entity is choking
					int iSimulationTicks = clamp(floorf((m_flSimTimeDelta * flTickInterval) + 0.5f), 1, 15);
					// How many ticks since entity was last simulated
					int iDeltaTicks = TIME_TO_TICKS(flDeltaTime);
					// How many ticks have passed since sim time - how many ticks they will choke
					int iSimulationDif = iDeltaTicks - iSimulationTicks;
					// Total angle difference divided by time = angle per unit of time (in this case seconds)
					float flAnglePerSec = velocityAngle / m_flSimTimeDelta;
			
					// Just velocity length
					float flVelocityLength2D = data.m_vecVelocity.Length2D();
				

					if (iSimulationDif >= 0)
					{
						do
						{
							if (iSimulationTicks > 0)
							{
								int counter = iSimulationTicks;
								do
								{
									// Add on the estimated angle difference
									float flExtrapolated = velocityDegree + (game_interfaces::Globals->interval_per_tick * flAnglePerSec);

									// Calculate the new velocity
									data.m_vecVelocity.x = cosf(DEG2RAD(flExtrapolated)) * flVelocityLength2D;
									data.m_vecVelocity.y = sinf(DEG2RAD(flExtrapolated)) * flVelocityLength2D;

									// Check if entity is in air
									bool InAir = !(ent->m_fFlags() & FL_ONGROUND);

									// Simulate the movement calculated
									SimulateMovement(data, InAir, ent);

									// Store the new velocity angle
									velocityDegree = flExtrapolated;

									data.SimulatedTime = ent->m_flSimulationTime() + game_interfaces::Globals->interval_per_tick;

									// One tick has been simulated, subtract this from the count
									--counter;
								} while (counter);
							}
							iSimulationDif -= iSimulationTicks;
						} while (iSimulationDif >= 0);
					}

					if (std::isfinite(data.m_vecSimulatedOrigin.x) && std::isfinite(data.m_vecSimulatedOrigin.y) && std::isfinite(data.m_vecSimulatedOrigin.z))
					{
					//	*ent->m_fFlags_ptr() = data.Flags;
						ent->SetAbsOrigin(data.m_vecSimulatedOrigin);
					
						for (int i = 0; i < ent->GetNumAnimOverlays(); i++)
						{
							animationlayer& layer = ent->GetAnimOverlay(i);
							layer = anim_layer_backup[i];
						}
					}
				
					ent->extrapolation_simulationtime = data.SimulatedTime;

					olddata.m_vecVelocity = data.m_vecVelocity;
					olddata.m_vecSimulatedOrigin = data.m_vecSimulatedOrigin;
					olddata.Flags = data.Flags;
				}
			}
		}
	}
}

