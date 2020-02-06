#include "HookManager.h"
#include "InterfaceManager.h"
#include "AntiAim.h"
#include "Aimbot.h"
#include "EPrediction.h"
#include "BunnyHop.h"
#include "Menu.h"
#include "MovementFix.h"
#include "MiscFunctions.h"
#include "LowerBodyTimer.h"
#include "LagFix.h"
#include "SecurityUtils.h"

#include <intrin.h> 

bool ResetNetChannelHook = true;

void hook_netchannel()
{
	if (game_interfaces::ClientState)
	{
		if (game_interfaces::ClientState->m_NetChannel())
		{
			if (!hooks::NetChannel_Hook)
			{
				hooks::NetChannel_Hook = new CVMTHookManager((PDWORD*)game_interfaces::ClientState->m_NetChannel());
				hooks::oSendDatagram = (SendDatagramFn)hooks::NetChannel_Hook->HookMethod((DWORD)hooks::SendDatagram_Hook, 46);
			}
			else {
				if (ResetNetChannelHook)
				{
					hooks::NetChannel_Hook->UnHook();
					hooks::NetChannel_Hook = nullptr;
					hooks::oSendDatagram = nullptr;

					if (HookValues::sequences.size() > 0)
					{
						HookValues::sequences.clear();
					}
					HookValues::lastincomingsequencenumber = 0.f;

					ResetNetChannelHook = false;
				}
				else {
					hooks::UpdateIncomingSequences();
				}
			}
		}
	}
}

int security_timer = 0;

bool __stdcall hooks::CreateMove_Hook(float frametime, CUserCmd* cmd)
{
	if (game_interfaces::Engine->Connected() && game_interfaces::Engine->InGame())
	{
		uintptr_t* FPointer; __asm {MOV FPointer, EBP }
		bool* pbSendPacket = (bool*)(*FPointer - 0x1C);
		bool& SendPacket = *pbSendPacket;

		entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());

		if (!cmd || !local)
		{
			return true;
		}

		if (local->m_iHealth() > 0 && !local->IsDormant() && local->IsPlayer() && cmd->command_number)
		{
			if (MenuValues::MenuOpen) // We don't wanna shoot when the menu is open, this is caused because of a bug that happens with the lockcursor hook
			{
				if (cmd->buttons & IN_ATTACK)
					cmd->buttons &= ~IN_ATTACK;
			}

			static ConVar* postprocess = game_interfaces::CVar->FindVar("mat_postprocess_enable"); // We initialize the post_process convar
			if (postprocess) postprocess->SetValue(!MenuControls::v_other_postprocess->GetState()); // We change the convar value to the value from the menu

			// [MAY CRASH] hook_netchannel(); // We have to do it here because the net_channel changes ingame

			Misc_Functions::UpdateClanTag();

			Misc_Functions::UpdatePrediction(local);

			baseweapon* localweapon = local->GetWeapon(); // We get the weapon from the localplayer
			Vector MovementVector = BunnyHop::update(cmd, local); // We initialize the movementvector that is used by the movement fix

			MovementFix::start(cmd); // We backup some usercmd values

			Prediction::start(cmd, local); // We backup some prediction values and start the prediction

			LBYTimer::Update();

			if (localweapon)
			{
				Aimbot::AimbotShooting = false;
				Aimbot::Aimbot(cmd, local, localweapon, MovementVector, SendPacket); // Aimbot update	
				if (MenuControls::a_norecoil->GetState() && local && localweapon)
				{
					Aimbot::NoRecoil(HookValues::RealAimPunchAngle, local, localweapon, cmd);
				}
				if (MenuControls::a_nospread->GetState() && local && localweapon)
				{
					NoSpread::NoSpread(local, localweapon, cmd);
				}
			}

			Prediction::stop(local, cmd); // We restore the prediction backup values 

			FakeLag::FakeLag(local, SendPacket);

			AntiAim::AntiAim(cmd, local, SendPacket); // AntiAim update

			//Misc_Functions::FakeWalk(cmd, SendPacket); // FakeWalk update
			MathUtils::AntiUntrusted_ClampAngles(cmd->viewangles); // AntiUntrusted

			MovementVector = Misc_Functions::CircleStrafe(cmd, local, MovementVector);

			MovementFix::end(cmd, MovementVector); // We restore the backup values

			MathUtils::AntiUntrusted_ClampAngles(cmd->viewangles); // AntiUntrusted	

			if (SendPacket)
			{
				AntiAim::UpdateGhosts(cmd);
			}

			Misc_Functions::UpdateGroundPitch(local);
		}


		if (security_timer < 30)
		{
			security_timer += 1;
		}
		else {
			security_timer = 0;
			SecurityUtils::check_guard();
		}

		Misc_Functions::LastUserCmd = cmd;

		return false;
	}
	return true;
}