#include "LowerBodyTimer.h"

#include "InterfaceManager.h"
#include "MemoryUtils.h"

float LBYTimer::old_curtime;
bool LBYTimer::lby_updated;
bool LBYTimer::was_moving;
bool LBYTimer::lby_can_break;
float LBYTimer::hold_time;

void LBYTimer::Update()
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local) // Local is not null pointer
	{
		if (local->m_iHealth() > 0 && !local->IsDormant()) // Local is alive
		{
			bool moving = local->m_vecVelocity().Length2D() > 0.1f; // Local is moving
			bool on_ground = local->m_fFlags() & FL_ONGROUND;
			float predict_time = was_moving ? 0.22f : 1.1f; // Next update

			if (!on_ground)
			{
				lby_updated = false; // LBY isn't updated
				lby_can_break = false; // LBY can't break
				old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
			}
			else {
				if (moving) // Local is moving
				{
					was_moving = true; // Was moving
					lby_can_break = false; // LBY can't break
					lby_updated = true; // LBY is updated

					old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
				}
				else {
					if (game_interfaces::Globals->curtime - old_curtime > predict_time) // LBY is updating
					{
						lby_updated = false; // LBY is not updated
						hold_time = 0.f; // We reset the timer for the next update
						was_moving = false; // Not moving anymore
						lby_can_break = true; // LBY can break

						old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
					}
					else {
						if (lby_can_break && hold_time == 0.f) // LBY is updated
						{
							old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
						}
						if (hold_time > 0.01f) // To make sure that antiaim updates the angle
						{
							hold_time = 0.f; // We reset the timer for the next time
							lby_can_break = false; // LBY can't break
							lby_updated = true; // LBY is updated
						}
						else {
							hold_time += 0.01f; // We increase the timer
						}
					}
				}
			}
		}
	}
}
