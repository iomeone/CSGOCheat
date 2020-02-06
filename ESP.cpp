
#include "Visuals.h"
#include "RenderUtils.h"
#include "InterfaceManager.h"
#include "Menu.h"
#include "Resolver.h"

ESP_CollisionBox ESP::GetRenderPoint(entity* ent)
{
	if (!ent)
		return ESP_CollisionBox(0, 0, 0, 0);
	if(!(ent->m_iHealth() > 0) || !ent->IsPlayer())
		return ESP_CollisionBox(0, 0, 0, 0);
	if (!ent->GetClientRenderable())
		return ESP_CollisionBox(0, 0, 0, 0);
	if (!ent->GetClientRenderable()->GetIClientUnknown())
		return ESP_CollisionBox(0, 0, 0, 0);
	if (!ent->GetCollideable())
		return ESP_CollisionBox(0, 0, 0, 0);

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector min = ent->GetCollideable()->OBBMins();
	Vector max = ent->GetCollideable()->OBBMaxs();

	Vector pointsTransformed[8];
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	for (int i = 0; i < 8; i++)
		MathUtils::VectorTransform(points[i], trans, pointsTransformed[i]);	

	Vector pos = ent->GetAbsOrigin(), flb, brt, blb, frt, frb, brb, blt, flt;

	if (!RenderUtils::WorldToScreen(pointsTransformed[3], flb) || !RenderUtils::WorldToScreen(pointsTransformed[5], brt)
		|| !RenderUtils::WorldToScreen(pointsTransformed[0], blb) || !RenderUtils::WorldToScreen(pointsTransformed[4], frt)
		|| !RenderUtils::WorldToScreen(pointsTransformed[2], frb) || !RenderUtils::WorldToScreen(pointsTransformed[1], brb)
		|| !RenderUtils::WorldToScreen(pointsTransformed[6], blt) || !RenderUtils::WorldToScreen(pointsTransformed[7], flt))
		return ESP_CollisionBox(0,0,0,0);

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;		// left
	float top = flb.y;		// top
	float right = flb.x;	// right
	float bottom = flb.y;	// bottom

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	return ESP_CollisionBox(left, bottom, right - left, top - bottom);
}

void ESP::DrawHitBoxes(entity* ent, Color col, float duration, matrix3x4_t* matrix)
{
	if (!ent)
		return;

	if (ent->IsDormant() || !(ent->m_iHealth() > 0))
		return;

	studiohdr_t* hdr = game_interfaces::ModelInfo->GetStudiomodel(ent->GetModel());
	if (hdr)
	{
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
		if (set)
		{
			for (int i = 0; i < set->numhitboxes; i++)
			{
				mstudiobbox_t* hitbox = set->GetHitbox(i);

				if (!hitbox)
					continue;

				if (!hitbox->bone)
					continue;

				if (!(hitbox->bbmax.Length() > 1))
					continue;

				Vector vMin, vMax;
				if (hitbox->m_flRadius != -1)
				{
					MathUtils::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
					MathUtils::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
					game_interfaces::DebugOverlay->AddCapsuleOverlay(vMin, vMax, hitbox->m_flRadius, col.r(), col.g(), col.b(), col.a(), duration);
				}
			}
		}
	}
}

void ESP::DrawBox(float BoxY, float BoxX, float BoxW, float BoxH, entity* ent, entity* local)
{
	if (!ent)
		return;

	if (!(ent->m_iHealth() > 0))
		return;

	float alpha = ent->IsDormant() && MenuControls::v_players_esp_dormant->GetState() ? (Visuals::ESPAlpha[ent->GetIndex()] >= 50 ? Visuals::ESPAlpha[ent->GetIndex()] : 50) : Visuals::ESPAlpha[ent->GetIndex()];

	RenderUtils::RenderOutlineRect(BoxX - 1, BoxY - 1, BoxW + 2, BoxH + 2, Color(25, 25, 25, alpha));
	RenderUtils::RenderOutlineRect(BoxX, BoxY, BoxW, BoxH, ent->m_iTeamNum() != local->m_iTeamNum() ? Color(MenuControls::v_players_esp_enemy_box->GetCustomColor().r(), MenuControls::v_players_esp_enemy_box->GetCustomColor().g(), MenuControls::v_players_esp_enemy_box->GetCustomColor().b(), alpha) : Color(MenuControls::v_players_esp_ally_box->GetCustomColor().r(), MenuControls::v_players_esp_ally_box->GetCustomColor().g(), MenuControls::v_players_esp_ally_box->GetCustomColor().b(), alpha));
	RenderUtils::RenderOutlineRect(BoxX + 1, BoxY + 1, BoxW - 2, BoxH - 2, Color(25, 25, 25, alpha));
}

void ESP::DrawHealthBar(float BoxY, float BoxX, float BoxW, float BoxH, entity* ent, entity* local)
{
	if (!ent)
		return;

	if (!(ent->m_iHealth() > 0))
		return;

	BoxX -= 5;

	float alpha = Visuals::ESPAlpha[ent->GetIndex()];

	float health = (float)ent->m_iHealth();
	float max_health = (float)ent->GetMaxHealth();
	float health_perc = health / max_health;
	float red_color = 255.f - (255.f * health_perc);
	float green_color = 255.f * health_perc;
	if (green_color < 0.f){green_color = 0;}
	if (red_color < 0.f){red_color = 0;}
	float calculated_box_height = ((float)BoxH * health_perc);
	float alpha_percent = alpha / 255.f;

	Color health_color(red_color, green_color, 0, alpha);

	float slip_center = calculated_box_height / 2.f;
	float split_height = (calculated_box_height / 2.f) * alpha_percent;

	if (MenuControls::v_players_esp_health->GetSelectedItem() == 1)
	{
		if (alpha < 255.f)
		{
			float alpha_percent = alpha / 255.f;

			RenderUtils::RenderRect(BoxX, BoxY + (BoxH - calculated_box_height) + slip_center, 3, split_height, health_color);
			RenderUtils::RenderRect(BoxX, BoxY + (BoxH - calculated_box_height) + slip_center - split_height, 3, split_height, health_color);		
			RenderUtils::RenderOutlineRect(BoxX, BoxY + (BoxH - calculated_box_height) + ((BoxH / 2.f) - split_height), 3, (split_height * 2), Color(0, 0, 0, alpha));
		}
		else {
			RenderUtils::RenderRect(BoxX, BoxY, 3, BoxH, Color(25, 25, 25, alpha));
			RenderUtils::RenderRect(BoxX, BoxY - calculated_box_height + BoxH, 3, calculated_box_height, health_color);
			RenderUtils::RenderOutlineRect(BoxX, BoxY, 3, BoxH, Color(0, 0, 0, alpha));
		}
	}

	if (MenuControls::v_players_esp_health->GetSelectedItem() == 2)
	{
		if (alpha < 255.f)
		{
			float height_per_section_per_split = (float)split_height / 10.f;

			RenderUtils::RenderRect(BoxX, BoxY + (BoxH - calculated_box_height) + slip_center, 3, split_height, health_color);
			RenderUtils::RenderRect(BoxX, BoxY + (BoxH - calculated_box_height) + slip_center - split_height, 3, split_height, health_color);

			RenderUtils::RenderOutlineRect(BoxX, BoxY + (BoxH - calculated_box_height) + ((BoxH / 2.f) - split_height), 3, (split_height * 2), Color(0, 0, 0, alpha));

			bool ignore_first = true;

			for (int i = 0; i < 10.f; i++)
			{
				if (ignore_first)
				{
					ignore_first = false;
				}
				else {
					RenderUtils::RenderLine(BoxX, BoxY + (BoxH - calculated_box_height) + slip_center + (height_per_section_per_split * i),
						BoxX + 3, BoxY + (BoxH - calculated_box_height) + slip_center + (height_per_section_per_split * i), Color(0, 0, 0, alpha));
				}
			}
			for (int i = 0; i < 10.f; i++)
			{
				RenderUtils::RenderLine(BoxX, BoxY + (BoxH - calculated_box_height) + slip_center - (height_per_section_per_split * i),
					BoxX + 3, BoxY + (BoxH - calculated_box_height) + slip_center - (height_per_section_per_split * i), Color(0, 0, 0, alpha));
			}
		}
		else {
			RenderUtils::RenderRect(BoxX, BoxY, 3, BoxH, Color(25, 25, 25, alpha));
			RenderUtils::RenderRect(BoxX, BoxY - calculated_box_height + BoxH, 3, calculated_box_height, health_color);
			RenderUtils::RenderOutlineRect(BoxX, BoxY, 3, BoxH, Color(0, 0, 0, alpha));

			float split_height = (calculated_box_height / 2.f) * alpha_percent;
			float height_per_section_per_split = (float)split_height / 10.f;

			bool ignore_first = true;

			for (int i = 0; i < 20.f; i++)
			{
				if (ignore_first)
				{
					ignore_first = false;
				}
				else {
					RenderUtils::RenderLine(BoxX, BoxY + (BoxH - calculated_box_height) + (height_per_section_per_split * i),
						BoxX + 3, BoxY + (BoxH - calculated_box_height) + (height_per_section_per_split * i), Color(0, 0, 0, alpha));
				}
			}

		}
	}
}

void ESP::DrawOther(float BoxY, float BoxX, float BoxW, float BoxH, entity* ent, entity* local)
{
	if (!ent)
		return;

	if (!(ent->m_iHealth() > 0))
		return;

	BoxY -= 5;

	float alpha = ent->IsDormant() && MenuControls::v_players_esp_dormant->GetState() ? (Visuals::ESPAlpha[ent->GetIndex()] >= 50 ? Visuals::ESPAlpha[ent->GetIndex()] : 50) : Visuals::ESPAlpha[ent->GetIndex()];
	float reload_animation_scale = 0.f;
	bool reloading = false;
	bool triggering_break = false;
	float triggering_scale = 0.f;

	for (int i = 0; i < ent->GetNumAnimOverlays(); i++)
	{
		animationlayer& layer = ent->GetAnimOverlay(i);
		if (layer.m_pOwner)
		{
			int activity = ent->GetSequenceActivity(layer.m_nSequence);
			if (activity >= 0)
			{
				if (activity == 979 && layer.m_flCycle <= 0.95f)
				{
					float scale = layer.m_flCycle / 0.95f;
					if (scale > 1)
						scale = 1.f;

					triggering_scale = scale;
					triggering_break = true;
				}
				if (activity == 967 && layer.m_flCycle <= 0.95f)
				{
					float scale = layer.m_flCycle / 0.95f;
					if (scale > 1)
						scale = 1.f;

					reload_animation_scale = scale;
					reloading = true;
				}
			}
		}
	}

	if (MenuControls::v_players_esp_name->GetState())
	{
		playerinfo player_info;
		if (game_interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &player_info))
		{
			wchar_t buffer[36];
			if (MultiByteToWideChar(CP_UTF8, 0, player_info.name, -1, buffer, 36) > 0) {
				std::wstring modified_string = std::wstring(buffer);
				float name_height = RenderUtils::GetTextHeight(modified_string, RenderUtils::Visuals_font);
				float name_width = RenderUtils::GetTextLenght(modified_string, RenderUtils::Visuals_font);

				RenderUtils::RenderText(BoxX + (BoxW / 2) - (name_width / 2), BoxY - (name_height / 2), Color(255,255,255, alpha), buffer, RenderUtils::Visuals_font);
			}
		}
		BoxY += 5;
	}

	if (MenuControls::v_players_esp_weapon->GetState())
	{
		baseweapon* weapon = ent->GetWeapon();
		if (weapon)
		{
			weaponinfo_t* weapon_info = weapon->GetCSWpnData();
			if (weapon_info)
			{
				float ammo = weapon->m_iClip1();
				float maxammo = weapon_info->m_iMaxClip;
				float ammo_scale = ammo / maxammo;

				if (reloading)
				{
					RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
					RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), (BoxW * reload_animation_scale), 3, Color(0, 237, 255, alpha));
					RenderUtils::RenderOutlineRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
				}
				else {
					RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
					RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), (BoxW * ammo_scale), 3, Color(0, 237, 255, alpha));
					RenderUtils::RenderOutlineRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
				}
				BoxY += 5;
			}
		}
	}

	if (MenuControls::a_resolver->GetState())
	{
		for (ResolverPlayer &rp : Resolver::Resolver_Players)
		{
			if (rp.PlayerIndex == ent->GetIndex())
			{
				float predict_time = rp.Spread.R_LBYTimer_was_moving ? 0.22f : 1.1f; // Next update
				float diff = game_interfaces::Globals->curtime - rp.Spread.R_LBYTimer_old_curtime;
				float scale = diff / predict_time;
				if (scale > 1.f)
					scale = 1.f;
				if (scale <= 0.f)
					scale = 1.f; // LBY is getting updates too fast

				RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
				RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), (BoxW * scale), 3, Color(90, 255, 93, alpha));
				RenderUtils::RenderOutlineRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));

				BoxY += 5;
			}
		}
		if (triggering_break)
		{
			RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
			RenderUtils::RenderRect(BoxX, (BoxY + BoxH + 2), (BoxW * triggering_scale), 3, Color(255, 135, 0, alpha));
			RenderUtils::RenderOutlineRect(BoxX, (BoxY + BoxH + 2), BoxW, 3, Color(0, 0, 0, alpha));
			BoxY += 5;
		}
	}

	if (MenuControls::v_players_esp_weapon->GetState())
	{
		baseweapon* weapon = ent->GetWeapon();
		if (weapon)
		{
			weaponinfo_t* weapon_info = weapon->GetCSWpnData();
			if (weapon_info)
			{
				if (sizeof(weapon_info->m_szWeaponName) > 0)
				{
					std::string s = weapon_info->m_szWeaponName;
					std::string replace = "weapon_";
					if (s.find(replace) != std::string::npos)
					{
						s.replace(s.find(replace), replace.length(), "");
					}
					std::string replace2 = "_";
					while (s.find(replace2) != std::string::npos)
					{
						s.replace(s.find(replace2), replace2.length(), "");
					}

					float weapon_name_height = RenderUtils::GetTextHeight(s, RenderUtils::Visuals_font);
					float weapon_name_width = RenderUtils::GetTextLenght(s, RenderUtils::Visuals_font);

					RenderUtils::RenderText(BoxX + (BoxW / 2) - (weapon_name_width / 2), BoxY + BoxH + 1, Color(255, 255, 255, alpha), s.c_str(), RenderUtils::Visuals_font);
				}
			}
		}
		BoxY += 5;
	}
}