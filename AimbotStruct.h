#pragma once

#include "Vector.h"

struct Hitscan_Settings
{
	Hitscan_Settings(bool p_backtrack, bool p_visible, bool only_basic, bool asap, bool exit_if_3_failed)
	{
		this->prioritize_backtrack = p_backtrack;
		this->prioritize_visible = p_visible;
		this->only_basic_hitboxes = only_basic;
		this->as_fast_as_possible = asap;
		this->exit_if_3_hitboxes_failed = exit_if_3_failed;
	}
	Hitscan_Settings()
	{

	}

	bool prioritize_backtrack = false;
	bool prioritize_visible = false;
	bool only_basic_hitboxes = false;
	bool as_fast_as_possible = false;
	bool exit_if_3_hitboxes_failed = false;
};

struct Hitscan_Result
{
	Hitscan_Result()
	{
		this->result = false;
		this->hitbox_pos = Vector(0, 0, 0);
		this->hitbox_id = -1;
		this->damage = 0;
	}
	Hitscan_Result(Vector hitbox_pos, int hitbox_id)
	{
		this->result = true;
		this->hitbox_pos = hitbox_pos;
		this->hitbox_id = hitbox_id;
		this->damage = 100;
	}
	Hitscan_Result(Vector hitbox_pos, int hitbox_id, float damage)
	{
		this->result = true;
		this->hitbox_pos = hitbox_pos;
		this->hitbox_id = hitbox_id;
		this->damage = damage;
	}
	float damage;
	bool result;
	Vector hitbox_pos;
	int hitbox_id;
};

struct AimbotHitbox
{
	AimbotHitbox()
	{
		this->id = -1;
		this->vec = Vector(0, 0, 0);
	}
	AimbotHitbox(int id, Vector vec)
	{
		this->id = id;
		this->vec = vec;
	}
	int id;
	Vector vec;
};

struct HitScanHitBox
{
	HitScanHitBox(int id, float damage, Vector p)
	{
		this->damage = damage;
		this->id = id;
		this->vec = p;
	}

	float damage;
	int id;
	Vector vec;
};

struct AimbotMultiPoint
{
	AimbotMultiPoint(Vector v, bool m)
	{
		this->Point = v;
		this->IsMultiPointed = m;
	}
	Vector Point;
	bool IsMultiPointed;

};