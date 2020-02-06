#include "Aimbot.h"
#include "Menu.h"
#include "AutoWall.h"
#include "LagCompensation.h"
#include "Resolver.h"
#include "AimbotStruct.h"

Hitscan_Result VisibleCheck(Vector pos, int hitbox, entity* ent, entity* local)
{
	if (ent->IsVisiblePosition(pos, local))
	{
		return Hitscan_Result(pos, hitbox);
	}

	return Hitscan_Result();
}

Hitscan_Result AutoWallCheck(Vector pos, int hitbox, entity* ent, entity* local)
{
	float autowall_damage = Autowall::GetDamage(pos, local, ent);
	if (autowall_damage >= MenuControls::a_mindamage->GetValue())
	{
		return Hitscan_Result(pos, hitbox);
	}
	return Hitscan_Result();
}

bool Aimbot::ExpensiveHitScan(entity* ent, entity* local, AimbotHitbox* HitBox)
{
	if (!local)
		return false;

	baseweapon* localweapon = local->GetWeapon();
	if (!localweapon)
		return false;

	bool use_autowall = MenuControls::a_autowall->GetState();
	bool use_backtrack = MenuControls::a_backtrack->GetState();

	bool should_body_aim = false;

	if (MenuControls::a_resolver->GetState())
	{
		for (int &h : MenuControls::a_resolver_bodyaim_if->GetSelectedItems())
		{
			switch (h)
			{
			case 0: // In Air
				if (!(ent->m_fFlags() & FL_ONGROUND))
				{
					should_body_aim = true;
				}
				break;
			case 1: //Unresolved
				if (Resolver::IsUnresolved(ent))
				{
					should_body_aim = true;
				}
				break;
			}
		}

		if (Aimbot::ShotsFired > MenuControls::a_resolver_bodyaim_after_shots->GetValue())
		{
			should_body_aim = true;
		}
	}

	Hitscan_Settings hitscan_settings;

	hitscan_settings.prioritize_backtrack = false;
	hitscan_settings.prioritize_visible = false;
	hitscan_settings.only_basic_hitboxes = false;
	hitscan_settings.as_fast_as_possible = false;
	hitscan_settings.exit_if_3_hitboxes_failed = false;

	std::vector<HitScanHitBox> valid_hitboxes_list;

	Vector result_hitbox_point = Vector(0, 0, 0);

	int result_hitbox_id = -1;
	int hitboxes_failed = 0;

	for (int &h : MenuControls::a_hitscan_settings->GetSelectedItems())
	{
		switch (h)
		{
		case 0: //Prioritize BackTrack
			hitscan_settings.prioritize_backtrack = true;
			break;
		case 1: //Prioritize Visible Hitboxes
			hitscan_settings.prioritize_visible = true;
			break;
		case 2: //Only Basic HitBoxes
			hitscan_settings.only_basic_hitboxes = true;
			break;
		case 3: //As Fast As Possible
			hitscan_settings.as_fast_as_possible = true;
			break;
		case 4: //If 3 Hitboxes Failed Exit
			hitscan_settings.exit_if_3_hitboxes_failed = true;
			break;
		}
	}

	UseBackTrack = false;

	if (hitscan_settings.prioritize_backtrack && use_backtrack)
	{
		AimbotBackTrackVector BackTrack = LC::GetVectorToShoot(ent, local);

		if (BackTrack.record.ID >= 0 && BackTrack.hitbox.vec.Length() > 0)
		{
			UseBackTrack = true;
			BackTrack_Record = BackTrack.record;
			*HitBox = BackTrack.hitbox;

			return true;
		}
	}
	else {
		std::vector<int> hitbox_list_to_use = hitscan_settings.only_basic_hitboxes ? VitalHitboxes() : GetTargetHitboxes();
		for (int hitbox_id : hitbox_list_to_use)
		{
			if (should_body_aim && hitbox_id < 2)
				continue;
			
			Vector HitboxVector = Aimbot::GetNormalPoint(ent, hitbox_id);
			
				if (hitscan_settings.prioritize_visible)
				{
					Hitscan_Result VisibleC = VisibleCheck(HitboxVector, hitbox_id, ent, local);
					if (VisibleC.result) // Success
					{
						if (hitscan_settings.as_fast_as_possible)
						{
							result_hitbox_id = VisibleC.hitbox_id;
							result_hitbox_point = VisibleC.hitbox_pos;
							break;
						}
						else {
							valid_hitboxes_list.push_back(HitScanHitBox(VisibleC.hitbox_id, 100.f, VisibleC.hitbox_pos));
						}
					}
					else {
						Hitscan_Result AutoWallC = AutoWallCheck(HitboxVector, hitbox_id, ent, local);
						if (AutoWallC.result)
						{
							if (hitscan_settings.as_fast_as_possible)
							{
								result_hitbox_id = AutoWallC.hitbox_id;
								result_hitbox_point = AutoWallC.hitbox_pos;
								break;
							}
							else {
								valid_hitboxes_list.push_back(HitScanHitBox(AutoWallC.hitbox_id, AutoWallC.damage, AutoWallC.hitbox_pos));
							}
						}
						else {
							// Hitbox is not hittable
							hitboxes_failed += 1;
							if (hitscan_settings.exit_if_3_hitboxes_failed && hitboxes_failed >= 3)
							{
								return false;
							}
						}
					}
				}
				else {
					Hitscan_Result AutoWallC = AutoWallCheck(HitboxVector, hitbox_id, ent, local);
					if (AutoWallC.result) // Success
					{
						if (hitscan_settings.as_fast_as_possible)
						{
							result_hitbox_id = AutoWallC.hitbox_id;
							result_hitbox_point = AutoWallC.hitbox_pos;
							break;
						}
						else {
							valid_hitboxes_list.push_back(HitScanHitBox(AutoWallC.hitbox_id, AutoWallC.damage, AutoWallC.hitbox_pos));
						}
					}
					else
					{
						Hitscan_Result VisibleC = VisibleCheck(HitboxVector, hitbox_id, ent, local);
						if (VisibleC.result)
						{
							if (hitscan_settings.as_fast_as_possible)
							{
								result_hitbox_id = VisibleC.hitbox_id;
								result_hitbox_point = VisibleC.hitbox_pos;
								break;
							}
							else {
								valid_hitboxes_list.push_back(HitScanHitBox(VisibleC.hitbox_id, 100.f, VisibleC.hitbox_pos));
							}
						}
						else {
							// Hitbox is not hittable
							hitboxes_failed += 1;
							if (hitscan_settings.exit_if_3_hitboxes_failed && hitboxes_failed >= 3)
							{
								return false;
							}
						}
					
				}
			}
		}
	}

	if (result_hitbox_point.Length() > 0.f && result_hitbox_id >= 0)
	{
		*HitBox = AimbotHitbox(result_hitbox_id, result_hitbox_point);
		return true;
	}
	else {
		if (valid_hitboxes_list.size() > 0)
		{
			Vector best_hitbox_pos = Vector(0,0,0);
			int best_hitbox_id = -1;
			float best_hitbox_damage = 0.f;

			for (HitScanHitBox ah : valid_hitboxes_list)
			{
				if (ah.damage > best_hitbox_damage)
				{
					best_hitbox_pos = ah.vec;
					best_hitbox_id = ah.id;
					best_hitbox_damage = ah.damage;
				}
			}

			if (best_hitbox_pos.Length() > 0.f && best_hitbox_id >= 0 && best_hitbox_damage > 0.f)
			{
				*HitBox = AimbotHitbox(best_hitbox_id, best_hitbox_pos);
				return true;
			}
		}
		else {
			if (use_backtrack)
			{
				AimbotBackTrackVector BackTrack = LC::GetVectorToShoot(ent, local);

				if (BackTrack.record.ID >= 0 && BackTrack.hitbox.vec.Length() > 0)
				{
					UseBackTrack = true;
					BackTrack_Record = BackTrack.record;
					*HitBox = BackTrack.hitbox;
					return true;
				}
			}
		}
	}

	return false;
}