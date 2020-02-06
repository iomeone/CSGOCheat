#include "MiscFunctions.h"

#include "InterfaceManager.h"
#include "Menu.h"
#include "Entity.h"

#include "AntiAim.h"

void Misc_Functions::SetClantag(const char* tag)
{
	static auto clantag_change_function = (int(__fastcall*)(const char*, const char*))MemoryUtils::FindBytePattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15");

	clantag_change_function(tag, tag);
}

int ground_timer = 0;
void Misc_Functions::UpdateGroundPitch(entity* local)
{
	if (local->GetAnimationState()->m_bInHitGroundAnimation && local->GetAnimationState()->m_flHeadHeightOrOffsetFromHittingGroundAnimation)
	{
		if (ground_timer == 0)
		{
			ground_timer = 1;
		}
		else {
			AntiAim::AntiAimAngles_Real.x = 0.0f;
			AntiAim::AntiAimAngles_Fake.x = 0.0f;
		}
	}
	else {
		ground_timer = 0;
	}
}

CUserCmd* Misc_Functions::LastUserCmd;
void Misc_Functions::UpdatePrediction(entity* local)
{
	//if (Misc_Functions::LastUserCmd)
	//{
	//	if (Misc_Functions::LastUserCmd->hasbeenpredicted) {
	//		local->tick_base = local->m_nTickBase();
	//	}
	//	else {
	//		++local->tick_base;
	//	}
	//}
	//else {
	//	local->tick_base = local->m_nTickBase();
	//}
	local->tick_base = local->m_nTickBase();
	/*if (game_interfaces::ClientState->m_nDeltaTick() >= 0)
	{
		game_interfaces::Prediction->Update(game_interfaces::ClientState->m_nDeltaTick(),
			game_interfaces::ClientState->m_nDeltaTick() > 0, game_interfaces::ClientState->last_command_ack(),
			(int)((DWORD)game_interfaces::ClientState->lastoutgoingcommand() + (DWORD)game_interfaces::ClientState->chokedcommands())
		);
	}*/
}

bool ShouldRemoveClanTag = false;

void Misc_Functions::UpdateClanTag()
{
	if (MenuControls::m_clantag->GetState())
	{
		std::string clan = "Spectre";

		static std::string current_clan = "";
		static int clan_next_timer = 0;

		Misc_Functions::SetClantag("EzFrags");
	}
}
int FakeLag::RandomFakeLagValue;
Vector FakeLag::LastChokePosition;
Vector FakeLag::LastChokeAngles;

void FakeLag::FakeLag(entity* local,bool& SendPacket)
{
	float lag_amount = 0.f;
	float velocity = (float)local->m_vecVelocity().Length() / 450.f;
	velocity = (float)clamp(velocity, 0.f, 1.f);

	switch (MenuControls::m_fakelag_mode->GetSelectedItem())
	{
	case 0: // None
		break;
	case 1: // Adaptive
		lag_amount = (float)velocity * (float)MenuControls::m_fakelag_max->GetValue();
		break;
	case 2: // Random
		break;
	case 3: // Max
		lag_amount = (float)MenuControls::m_fakelag_max->GetValue();
		break;
	default: 
		break;
	}

//	std::cout << "SendPacket: " << SendPacket << std::endl;

	if (lag_amount > 0.f || MenuControls::m_fakelag_mode->GetSelectedItem() == 2)
	{
		if (MenuControls::m_fakelag_mode->GetSelectedItem() == 2)
		{
			if (FakeLag::RandomFakeLagValue < RandomFakeLagValue)
			{
				FakeLag::RandomFakeLagValue++;
				SendPacket = false;
			}
			else {
				FakeLag::RandomFakeLagValue = 0;
				FakeLag::LastChokeAngles = local->GetAbsAngles();
				FakeLag::LastChokePosition = local->GetAbsOrigin();

				RandomFakeLagValue = MathUtils::GetRandom(1, MenuControls::m_fakelag_max->GetValue());
				SendPacket = true;
			}
		}
		else {
			bool value = (FakeLag::RandomFakeLagValue < lag_amount) ? false : true;
			
			if (value)
			{
				FakeLag::LastChokeAngles = local->GetAbsAngles();
				FakeLag::LastChokePosition = local->GetAbsOrigin();
				FakeLag::RandomFakeLagValue = 0;
			}
			else {
				FakeLag::RandomFakeLagValue += 1;
			}

			SendPacket = value;
		}
	}
}

void Misc_Functions::UpdateNightMode()
{
	static bool nightmode_was_on = false;
	static int last_nightmode_value = MenuControls::v_other_nightmode_value->GetValue();
	static ConVar* r_drawspecificstaticprop = game_interfaces::CVar->FindVar("r_drawspecificstaticprop");
	static SpoofedConvar* r_drawspecificstaticprop_s = new SpoofedConvar(r_drawspecificstaticprop);

	// NightMode
	if (nightmode_was_on && !MenuControls::v_other_nightmode->GetState() || MenuControls::v_other_nightmode->GetState() && !nightmode_was_on || MenuControls::v_other_nightmode->GetState() && last_nightmode_value != MenuControls::v_other_nightmode_value->GetValue())
	{
		last_nightmode_value = MenuControls::v_other_nightmode_value->GetValue();

		if (r_drawspecificstaticprop_s)
		{
			r_drawspecificstaticprop_s->SetInt(MenuControls::v_other_nightmode->GetState() ? 1 : 0);
		}

		float main_night_value = (float)MenuControls::v_other_nightmode_value->GetValue();
		main_night_value /= 100;
		float prop_night_value = main_night_value + 0.20f;
		if (prop_night_value > 1.f)
			prop_night_value = 1.f;
		float sky_night_value = main_night_value - 0.20f;
		if (sky_night_value < 0.1f)
			sky_night_value = 0.1f;

		for (int i = game_interfaces::MaterialSystem->FirstMaterial(); i != game_interfaces::MaterialSystem->InvalidMaterial(); i = game_interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial* material = game_interfaces::MaterialSystem->GetMaterial(i);
			if (material)
			{
				if (!material->IsErrorMaterial())
				{
					if (strstr(material->GetTextureGroupName(), "World"))
					{
						if (MenuControls::v_other_nightmode->GetState())
						{
							nightmode_was_on = true;
							material->ColorModulate(main_night_value, main_night_value, main_night_value);
						}
						else {
							nightmode_was_on = false;
							material->ColorModulate(1.f, 1.f, 1.f);
						}
					}
					if (strstr(material->GetTextureGroupName(), "StaticProp"))
					{
						if (MenuControls::v_other_nightmode->GetState())
						{
							nightmode_was_on = true;
							material->ColorModulate(prop_night_value, prop_night_value, prop_night_value);
						}
						else {
							nightmode_was_on = false;
							material->ColorModulate(1.f, 1.f, 1.f);
						}
					}
					if (strstr(material->GetTextureGroupName(), "SkyBox textures"))
					{
						if (MenuControls::v_other_nightmode->GetState())
						{
							nightmode_was_on = true;
							material->ColorModulate(sky_night_value, sky_night_value, sky_night_value);
						}
						else {
							nightmode_was_on = false;
							material->ColorModulate(1.f, 1.f, 1.f);
						}
					}
				}
			}
		}
	}
}

void Misc_Functions::FakeWalk(CUserCmd* cmd, bool& SendPacket)
{
	static int choked = 0;
	if (MenuControls::m_fakewalk->GetState() && cmd->buttons & IN_SPEED)
	{
		if (choked < 7)
		{
			SendPacket = false;
		}
		else {
			SendPacket = true;
		}

		choked = choked > 7 ? 0 : choked + 1;
		cmd->forwardmove = choked < 2 || choked > 5 ? 0 : cmd->forwardmove;
		cmd->sidemove = choked < 2 || choked > 5 ? 0 : cmd->sidemove;
		SendPacket = choked < 1;
	}
}

bool Misc_Functions::ThirdPersonSwitch = false;

void Misc_Functions::ThirdPerson()
{
	static bool ForceUpdate = false;
	static bool ThirdPersonKeyPressed = false;
	static int Timer = 0;

	game_interfaces::CVar->FindVar("cam_idealyaw")->SetValue(1);
	game_interfaces::CVar->FindVar("cam_idealpitch")->SetValue(1);
	static ConVar* sv_cheats = game_interfaces::CVar->FindVar("sv_cheats");
	static SpoofedConvar* spoofed_cheats = new SpoofedConvar(sv_cheats);
	spoofed_cheats->SetFlags(FCVAR_CHEAT);
	spoofed_cheats->SetInt(1);

	if (GetAsyncKeyState(MenuControls::m_thirdperson_bind->GetKey()) && !ThirdPersonKeyPressed)
	{
		ThirdPersonSwitch = !ThirdPersonSwitch;
		ForceUpdate = true;
		ThirdPersonKeyPressed = true;
	}
	else {
		if (!GetAsyncKeyState(MenuControls::m_thirdperson_bind->GetKey()))
		{
			ThirdPersonKeyPressed = false;
		}
	}

	if (ForceUpdate)
	{
		entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
		if (local)
		{
			if (local->m_iHealth() > 0)
			{
				game_interfaces::Engine->ClientCmd_Unrestricted(ThirdPersonSwitch ? "thirdperson" : "firstperson", true);
			}
		}
		ForceUpdate = false;
	}

	/*if (ForceUpdate)
	{
		ForceUpdate = false;
		entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
		if (local)
		{
			if (local->m_iHealth() > 0)
			{
				game_interfaces::Engine->ClientCmd_Unrestricted(ThirdPersonSwitch ? "thirdperson" : "firstperson", true);
			}
			else {
				game_interfaces::Engine->ClientCmd_Unrestricted("firstperson", true);
				if (Timer > 500)
				{
					Timer = 0;
					local->SetObserverMode(5);
				}
				else {
					Timer += 1;
				}
			}
		}
	}*/
}