#include "Visuals.h"
#include "Entity.h"
#include "InterfaceManager.h"
#include "RenderUtils.h"
#include "Menu.h"
#include "Resolver.h"
#include "LagCompensation.h"
#include "MemoryUtils.h"
#include <iostream>
#include "AntiAim.h"
#include "AutoWall.h"
#include "MiscFunctions.h"
#include "LowerBodyTimer.h"
#include "GrenadePrediction.h"
#include "MiscFunctions.h"
#include "Aimbot.h"

int Visuals::ESPAlpha[100];
int Visuals::HitMarker_Alpha = 0;
std::vector<ESPInfo> Visuals::ESPInfo_List;
float offscreen_alpha = 0.f;
bool offscreen_change_boolean = false;

namespace Materials
{
	IMaterial* material_normal;
	IMaterial* material_normal_ignorez;

	IMaterial* material_flat;
	IMaterial* material_flat_ignorez;

	IMaterial* material_wireframe;
	IMaterial* material_wireframe_ignorez;
}

void Visuals::SceneEnd_Render()
{
	float LowerBodyColor[3] = {
		(float)MenuControls::v_other_antiaimghost_lby->GetCustomColor().r() / 255.f,
		(float)MenuControls::v_other_antiaimghost_lby->GetCustomColor().g() / 255.f,
		(float)MenuControls::v_other_antiaimghost_lby->GetCustomColor().b() / 255.f
	};

	float FakeColor[3] = {
		(float)MenuControls::v_other_antiaimghost_fake->GetCustomColor().r() / 255.f,
		(float)MenuControls::v_other_antiaimghost_fake->GetCustomColor().g() / 255.f,
		(float)MenuControls::v_other_antiaimghost_fake->GetCustomColor().b() / 255.f
	};

	float FakeLagColor[3] = {
		(float)MenuControls::v_other_fakelag_ghost->GetCustomColor().r() / 255.f,
		(float)MenuControls::v_other_fakelag_ghost->GetCustomColor().g() / 255.f,
		(float)MenuControls::v_other_fakelag_ghost->GetCustomColor().b() / 255.f
	};

	float Enemy_ChamsColor[3] = {
		(float)MenuControls::v_players_esp_enemy_chams->GetCustomColor().r() / 255.f,
		(float)MenuControls::v_players_esp_enemy_chams->GetCustomColor().g() / 255.f,
		(float)MenuControls::v_players_esp_enemy_chams->GetCustomColor().b() / 255.f
	};

	float Ally_ChamsColor[3] = {
		(float)MenuControls::v_players_esp_ally_chams->GetCustomColor().r() / 255.f,
		(float)MenuControls::v_players_esp_ally_chams->GetCustomColor().g() / 255.f,
		(float)MenuControls::v_players_esp_ally_chams->GetCustomColor().b() / 255.f
	};

	IMaterial* chams_material = Materials::material_normal;
	IMaterial* chams_material_ignorez = Materials::material_normal;

	switch (MenuControls::v_players_esp_chams_mode->GetSelectedItem())
	{
	case 0: // Normal
		chams_material = Materials::material_normal;
		chams_material_ignorez = Materials::material_normal_ignorez;
		break;
	case 1: // Flat
		chams_material = Materials::material_flat;
		chams_material_ignorez = Materials::material_flat_ignorez;
		break;
	case 2: // Wireframe
		chams_material = Materials::material_wireframe;
		chams_material_ignorez = Materials::material_wireframe_ignorez;
		break;
	case 3: // Transparent
		chams_material = Materials::material_normal;
		chams_material_ignorez = Materials::material_normal_ignorez;
		break;
	case 4: // Hitboxes
			//RenderHitboxes(model_ent, model_ent->m_iTeamNum() == local->m_iTeamNum() ? Color(Ally_ChamsColor[0] * 255.f, Ally_ChamsColor[1] * 255.f, Ally_ChamsColor[2] * 255.f) : Color(Enemy_ChamsColor[0] * 255.f, Enemy_ChamsColor[1] * 255.f, Enemy_ChamsColor[2] * 255.f));
	default:
		break;
	}

	RenderableInstance_t instance;
	instance.m_nAlpha = (uint8_t)(1.f * 255.0f);

	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local)
	{
		if (MenuControls::v_players_esp_enable->GetState())
		{
			for (int i = 0; i < game_interfaces::Globals->maxClients; i++) // Player loop
			{
				entity* ent = game_interfaces::EntityList->GetClientEntity(i); // We get the entity from the entity list
				if (ent) // If entity is not a nullptr
				{
					if (ent->IsPlayer() && !ent->IsDormant() && ent->m_iHealth() > 0) // If entity is a valid player
					{
						if (MenuControls::a_backtrack->GetState() && MenuControls::v_players_esp_drawbacktrack->GetSelectedItem() == 2)
						{
							if (!local->IsDormant() && local->m_iHealth() > 0)
							{
								LC::DrawBackTrack(ent, local);
							}
						}
						bool team_enabled = ent->m_iTeamNum() == local->m_iTeamNum() ? MenuControls::v_players_esp_ally_enable->GetState() : MenuControls::v_players_esp_enemy_enable->GetState();
						if (team_enabled)
						{
							if (ent->m_iTeamNum() == local->m_iTeamNum() && MenuControls::v_players_esp_ally_chams->GetState() || ent->m_iTeamNum() != local->m_iTeamNum() && MenuControls::v_players_esp_enemy_chams->GetState())
							{
								if (MenuControls::v_players_esp_chams_mode->GetSelectedItem() < 4)
								{
									game_interfaces::RenderView->SetColorModulation(ent->m_iTeamNum() == local->m_iTeamNum() ? Ally_ChamsColor : Enemy_ChamsColor);
									game_interfaces::ModelRender->ForcedMaterialOverride(chams_material_ignorez);
									ent->DrawModel(0x1, instance);

									game_interfaces::RenderView->SetColorModulation(ent->m_iTeamNum() == local->m_iTeamNum() ? Ally_ChamsColor : Enemy_ChamsColor);
									game_interfaces::ModelRender->ForcedMaterialOverride(chams_material);

									ent->DrawModel(0x1, instance);

									game_interfaces::ModelRender->ForcedMaterialOverride(nullptr);
								}
							}
						}
					}
				}
			}
		}

		if (local->m_iHealth() > 0 && !local->IsDormant())
		{
			// AntiAim Angles Visualization
			if (MenuControls::a_antiaim->GetState())
			{
				if (MenuControls::v_other_antiaimghost_fake->GetState())
				{
					local->SetAbsAngles(Vector(0, AntiAim::AntiAimAngles_Fake.y, 0));

					game_interfaces::RenderView->SetColorModulation(FakeColor);
					game_interfaces::ModelRender->ForcedMaterialOverride(chams_material);
					local->DrawModel(0x1, instance);
					game_interfaces::ModelRender->ForcedMaterialOverride(nullptr);
					local->SetAbsAngles(Vector(0, AntiAim::AntiAimAngles_Real.y, 0));
				}
				if (MenuControls::v_other_antiaimghost_lby->GetState())
				{
					local->SetAbsAngles(Vector(0, local->m_flLowerBodyYawTarget(), 0));

					game_interfaces::RenderView->SetColorModulation(LowerBodyColor);
					game_interfaces::ModelRender->ForcedMaterialOverride(chams_material);
					local->DrawModel(0x1, instance);
					game_interfaces::ModelRender->ForcedMaterialOverride(nullptr);
					local->SetAbsAngles(Vector(0, AntiAim::AntiAimAngles_Real.y, 0));
				}
			}

			if (MenuControls::v_other_fakelag_ghost->GetState() && MenuControls::m_fakelag_max->GetValue() > 0.f && MenuControls::m_fakelag_mode->GetSelectedItem() > 0)
			{
				Vector backup_origin = local->GetAbsOrigin();
				Vector backup_angles = local->GetAbsAngles();

				local->SetAbsAngles(Vector(0, FakeLag::LastChokeAngles.y, 0));
				local->SetAbsOrigin(FakeLag::LastChokePosition);

				game_interfaces::RenderView->SetColorModulation(FakeLagColor);
				game_interfaces::ModelRender->ForcedMaterialOverride(chams_material);
				local->DrawModel(0x1, instance);
				game_interfaces::ModelRender->ForcedMaterialOverride(nullptr);
				local->SetAbsAngles(Vector(0, backup_angles.y, 0));
				local->SetAbsOrigin(backup_origin);
			}
		}
	}
}

void Visuals::DrawOnScreenRadar(entity* local, entity* ent)
{
	if (!local || local->m_iTeamNum() == ent->m_iTeamNum())
		return;

	RECT Screen = RenderUtils::Viewport();
	int XMiddle = Screen.left + (Screen.right / 2);
	int YMiddle = Screen.top + (Screen.bottom / 2);

	Vector New_HitBox;
	if (ent->GetHitboxPosition(0, New_HitBox))
	{
		Vector Result, Delta = New_HitBox - local->GetEyePosition();
		MathUtils::VectorAngles(Delta, Result);

		Vector client_viewangles;
		game_interfaces::Engine->GetViewAngles(client_viewangles);

		if (ent->IsDormant())
		{
			RenderUtils::RotatedTriangle(Vector2D(XMiddle, YMiddle), 150.f, MathUtils::degrees_to_radians(client_viewangles.y - Result.y - 90), Color(MenuControls::v_players_esp_offscreen->GetCustomColor().r(), MenuControls::v_players_esp_offscreen->GetCustomColor().g(), MenuControls::v_players_esp_offscreen->GetCustomColor().b(), Visuals::ESPAlpha[ent->GetIndex()]));
		}
		else {
			RenderUtils::RotatedTriangle(Vector2D(XMiddle, YMiddle), 150.f, MathUtils::degrees_to_radians(client_viewangles.y - Result.y - 90), Color(MenuControls::v_players_esp_offscreen->GetCustomColor().r(), MenuControls::v_players_esp_offscreen->GetCustomColor().g(), MenuControls::v_players_esp_offscreen->GetCustomColor().b(), offscreen_alpha));
		}
	}
}

std::vector<std::string> Visuals::GetEspInfo(int idx)
{
	std::vector<std::string> new_string_list;
	for (ESPInfo &ei : Visuals::ESPInfo_List)
	{
		if (ei.index == idx)
		{
			new_string_list.push_back(ei.text);
		}
	}
	return new_string_list;
}

void DrawSoundEsp()
{
	static CUtlVector< SndInfo_t > sndList;
	sndList.RemoveAll();
	game_interfaces::EngineSound->GetActiveSounds(sndList);

	for (int i = 0; i < sndList.Count(); i++)
	{
		SndInfo_t sndInfo = sndList.Element(i);

		Vector vScreen, vScreen2;

		if (RenderUtils::WorldToScreen(*sndInfo.m_pOrigin, vScreen))
		{
			if (sndInfo.m_nSoundSource != game_interfaces::Engine->GetLocalPlayer())
			{
				entity* ent = game_interfaces::EntityList->GetClientEntity(sndInfo.m_nSoundSource);
				if (ent)
				{
					if (!ent->IsDormant() && ent->m_iHealth() > 0 && ent->IsPlayer())
					{
						if (RenderUtils::WorldToScreen(ent->GetAbsOrigin(), vScreen2))
						{
							Vector or = *sndInfo.m_pOrigin;
							float dist = or .DistTo(ent->GetAbsOrigin());
							if (dist <= 5000.f)
							{
								RenderUtils::RenderLine(vScreen.x, vScreen.y, vScreen2.x, vScreen2.y, Color::White());
							}
						}
					}
				}
			}
		}
	}
}

void Visuals::PaintTraverse_Render()
{
	if (!Materials::material_normal) // We create the materials for the chams..
	{
		Materials::material_normal = MaterialUtils::CreateMaterial(false, false, false);
		Materials::material_normal_ignorez = MaterialUtils::CreateMaterial(false, true, false);

		Materials::material_flat = MaterialUtils::CreateMaterial(true, false, false);
		Materials::material_flat_ignorez = MaterialUtils::CreateMaterial(true, true, false);

		Materials::material_wireframe = MaterialUtils::CreateMaterial(false, false, true);
		Materials::material_wireframe_ignorez = MaterialUtils::CreateMaterial(false, true, true);
	}

	RECT Screen = RenderUtils::Viewport();
	int XMiddle = Screen.left + (Screen.right / 2);
	int YMiddle = Screen.top + (Screen.bottom / 2);

	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer()); // LocalPlayer
	if (local)
	{
		if (Visuals::ESPInfo_List.size() > 0)
		{
			Visuals::ESPInfo_List.clear(); // We clear
		}

		if (MenuControls::v_players_esp_sound->GetState())
		{
			DrawSoundEsp();
		}

		// Offscreen alpha update
		if (offscreen_change_boolean)
		{
			offscreen_alpha += 1.f;
			if (offscreen_alpha > 255.f)
				offscreen_change_boolean = false;
		}
		else {
			offscreen_alpha -= 1.f;
			if (offscreen_alpha < 1.f)
				offscreen_change_boolean = true;
		}

		for (int i = 0; i < game_interfaces::Globals->maxClients; i++) // Player Loop
		{
			if (i == game_interfaces::Engine->GetLocalPlayer())
				continue;

			entity* ent = game_interfaces::EntityList->GetClientEntity(i);
			if (ent)
			{
				if (ent->IsPlayer() && ent->m_iHealth() > 0)
				{

					if (!ent->IsDormant())
					{
						if (Visuals::ESPAlpha[ent->GetIndex()] < 255)
						{
							Visuals::ESPAlpha[ent->GetIndex()] += 1;
						}
					}
					else {
						if (Visuals::ESPAlpha[ent->GetIndex()] > 0)
						{
							Visuals::ESPAlpha[ent->GetIndex()] -= 1;
						}
					}


					if (MenuControls::v_players_esp_dormant->GetState() && ent->IsDormant() || !ent->IsDormant() || Visuals::ESPAlpha[ent->GetIndex()] > 0)
					{
						bool team_enabled = ent->m_iTeamNum() == local->m_iTeamNum() ? MenuControls::v_players_esp_ally_enable->GetState() : MenuControls::v_players_esp_enemy_enable->GetState();
						if (MenuControls::v_players_esp_enable->GetState() && team_enabled)
						{

							if (!ent->IsDormant())
							{
								if (MenuControls::a_resolver->GetState())
								{
									Resolver::Draw(ent);
								}
								if (MenuControls::a_backtrack->GetState() && MenuControls::v_players_esp_drawbacktrack->GetSelectedItem() == 1)
								{
									if (!local->IsDormant() && local->m_iHealth() > 0)
									{
										LC::DrawBackTrack(ent, local);
									}
								}
							}

							if (ent->m_ArmorValue() > 0)
							{
								Visuals::ESPInfo_List.push_back(ESPInfo(ent->GetIndex(), ent->m_bHasHelmet() ? "HK" : "K"));
							}

							ESP_CollisionBox calc_box = ESP::GetRenderPoint(ent);

							if (calc_box.x > 0 && calc_box.y > 0 && calc_box.w > 0 && calc_box.h > 0) // 3D to 2D
							{
								if (MenuControls::v_players_esp_ally_box->GetState() && local->m_iTeamNum() == ent->m_iTeamNum() || MenuControls::v_players_esp_enemy_box->GetState() && local->m_iTeamNum() != ent->m_iTeamNum())
								{
									ESP::DrawBox(calc_box.y, calc_box.x, calc_box.w, calc_box.h, ent, local);
								}
								if (MenuControls::v_players_esp_health->GetSelectedItem() > 0)
								{
									ESP::DrawHealthBar(calc_box.y, calc_box.x, calc_box.w, calc_box.h, ent, local);
								}

								ESP::DrawOther(calc_box.y, calc_box.x, calc_box.w, calc_box.h, ent, local);

								if (MenuControls::v_players_esp_info->GetState())
								{
									float string_list_scale = 0.f;
									for (std::string &s : GetEspInfo(ent->GetIndex()))
									{
										RenderUtils::RenderText(calc_box.x + calc_box.w + 5.f, calc_box.y + string_list_scale, Color(255, 255, 255, Visuals::ESPAlpha[ent->GetIndex()]), s.c_str(), RenderUtils::Visuals_font);
										string_list_scale += (RenderUtils::GetTextHeight(s, RenderUtils::Visuals_font) + 1.5f);
									}
								}



								for (AimbotMultiPoint mp : Aimbot::GetMultiPoints(ent, 0))
								{
									Vector w2s;
									bool visible = ent->IsVisiblePosition(mp.Point, local);
									if (RenderUtils::WorldToScreen(mp.Point, w2s))
									{
										RenderUtils::RenderRect(w2s.x - 5, w2s.y - 5, 10, 10, visible ? Color::Red() : Color::White());
									}
								}





							}
							else {
								if (MenuControls::v_players_esp_offscreen->GetState())
								{
									DrawOnScreenRadar(local, ent);
								}
							}
						}
					}
				}
			}
		}

		if (local->m_iHealth() > 0 && !local->IsDormant())
		{
			baseweapon* local_weapon = local->GetWeapon();
			if (local_weapon)
			{
				if (MenuControls::v_other_grenade_prediction->GetState())
				{
					GrenadePred::GrenadePrediction(local, local_weapon);
				}
			}

			if (MenuControls::v_other_noscope->GetState() && local->m_bIsScoped())
			{
				RenderUtils::RenderLine(Screen.left, YMiddle, Screen.right, YMiddle, Color::Black()); // Left - Right
				RenderUtils::RenderLine(XMiddle, Screen.top, XMiddle, Screen.bottom, Color::Black()); // Up - Down
			}

			if (MenuControls::v_other_hitmarker->GetState())
			{
				if (HitMarker_Alpha > 0)
				{
					float HitMarker_Line_Separation = 6;
					float HitMarker_Line_Lenght = HitMarker_Line_Separation + 8;

					RenderUtils::RenderLine(XMiddle - HitMarker_Line_Separation, YMiddle - HitMarker_Line_Separation, XMiddle - HitMarker_Line_Lenght, YMiddle - HitMarker_Line_Lenght, Color(MenuControls::v_other_hitmarker->GetCustomColor().r(), MenuControls::v_other_hitmarker->GetCustomColor().g(), MenuControls::v_other_hitmarker->GetCustomColor().b(), HitMarker_Alpha)); // Left UP Corner
					RenderUtils::RenderLine(XMiddle + HitMarker_Line_Separation, YMiddle + HitMarker_Line_Separation, XMiddle + HitMarker_Line_Lenght, YMiddle + HitMarker_Line_Lenght, Color(MenuControls::v_other_hitmarker->GetCustomColor().r(), MenuControls::v_other_hitmarker->GetCustomColor().g(), MenuControls::v_other_hitmarker->GetCustomColor().b(), HitMarker_Alpha)); // Right DOWN Corner
					RenderUtils::RenderLine(XMiddle - HitMarker_Line_Separation, YMiddle + HitMarker_Line_Separation, XMiddle - HitMarker_Line_Lenght, YMiddle + HitMarker_Line_Lenght, Color(MenuControls::v_other_hitmarker->GetCustomColor().r(), MenuControls::v_other_hitmarker->GetCustomColor().g(), MenuControls::v_other_hitmarker->GetCustomColor().b(), HitMarker_Alpha)); // Left DOWN Corner
					RenderUtils::RenderLine(XMiddle + HitMarker_Line_Separation, YMiddle - HitMarker_Line_Separation, XMiddle + HitMarker_Line_Lenght, YMiddle - HitMarker_Line_Lenght, Color(MenuControls::v_other_hitmarker->GetCustomColor().r(), MenuControls::v_other_hitmarker->GetCustomColor().g(), MenuControls::v_other_hitmarker->GetCustomColor().b(), HitMarker_Alpha)); // Right UP Corner

					HitMarker_Alpha -= 2;
				}
			}

			if (MenuControls::v_other_crosshair->GetState())
			{
				RenderUtils::RenderLine(XMiddle - 5, YMiddle, XMiddle + 5, YMiddle, Color(MenuControls::v_other_crosshair->GetCustomColor().r(), MenuControls::v_other_crosshair->GetCustomColor().g(), MenuControls::v_other_crosshair->GetCustomColor().b(), 255)); // Left - Right
				RenderUtils::RenderLine(XMiddle, YMiddle - 5, XMiddle, YMiddle + 5, Color(MenuControls::v_other_crosshair->GetCustomColor().r(), MenuControls::v_other_crosshair->GetCustomColor().g(), MenuControls::v_other_crosshair->GetCustomColor().b(), 255)); // Up - Down
			}

			//RenderUtils::RenderText(5, Screen.bottom - 60, LBYTimer::lby_updated ? Color(128, 184, 43, 255) : Color(255, 50, 0, 255), "LBY", RenderUtils::Bold_font);
		}
	}
}