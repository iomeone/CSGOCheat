#include "BunnyHop.h"

#include "InterfaceManager.h"
#include "Menu.h"

bool AutoStraferFlip; 
float AutoStraferFlipTimer;

Vector BunnyHop::update(CUserCmd* cmd, entity* local)
{
	Vector MovementVector;
	game_interfaces::Engine->GetViewAngles(MovementVector); // We get the real viewangles

	if (MenuControls::m_bunnyhop->GetState()) // BunnyHop activated
	{
		if (GetAsyncKeyState(VK_SPACE)) // Pressing the space bar
		{
			static bool Last_Jumped = false;
			static bool Should_Fake = false;

			if (!Last_Jumped && Should_Fake) 
			{
				Should_Fake = false;
				cmd->buttons |= IN_JUMP;
			}
			else if (cmd->buttons & IN_JUMP)
			{
				if (local->m_fFlags() & FL_ONGROUND)
				{
					Last_Jumped = true;
					Should_Fake = true;
				}
				else {
					cmd->buttons &= ~IN_JUMP;
					Last_Jumped = false;
				}
			}
			else {
				Last_Jumped = false;
				Should_Fake = false;
			}

			if (cmd->mousedx > 25.f)
			{
				MovementVector.y += 1; // We change the movement 
				cmd->sidemove = 450; // We move to the right
			}
			else {
				if (cmd->mousedx < -25.f)
				{
					MovementVector.y -= 1; // We change the movement 
					cmd->sidemove = -450; // We move to the left
				}
				else {
					if (!AutoStraferFlip)
					{
						MovementVector.y -= 1; // We change the movement 
						cmd->sidemove = -450; // We move to the left
					}
					else {
						MovementVector.y += 1; // We change the movement 
						cmd->sidemove = 450; // We move to the right
					}
				}
			}

			if (AutoStraferFlipTimer >= MenuControls::m_bunnyhop_delay->GetValue())
			{
				AutoStraferFlip = !AutoStraferFlip; // Change the boolean state
				AutoStraferFlipTimer = 0;
			}
			else {
				AutoStraferFlipTimer += 1.f;
			}
		}
	}

	return MovementVector; // Modified vector
}
