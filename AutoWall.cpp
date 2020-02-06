#include "AutoWall.h"
#include "MemoryUtils.h"
#include "InterfaceManager.h"

//	bool __fastcall IsBreakableEntity(CBaseEntity *this, float a2)
//	{
//		int v2; // esi@1
//		int v3; // eax@5
//		int v4; // eax@9
//		int v6; // eax@19
//		float v7; // ST18_4@20
//
//		v2 = a1;
//
//		if ( !a1 )
//			return 0;
//
//		if ( *(_DWORD *)(a1 + 252) >= 0 || (*(int (**)(void))(*(_DWORD *)a1 + 488))() <= 0 )
//		{
//			if ( *(_BYTE *)(v2 + 636) != 2 )
//			return 0;
//			v3 = *(_DWORD *)(v2 + 1136);
//			if ( v3 != 17 && v3 != 6 && v3 )
//			return 0;
//			if ( *(_DWORD *)(v2 + 252) > 200 )
//			return 0;
//			v4 = sub_107FDFD7(v2, 0, &off_10AD5974, &off_10AD7D34, 0);
//			if ( v4 )
//			{
//				if ( (**(int (__thiscall ***)(_DWORD))v4)(v4) != 1 )
//			return 0;
//			}
//		else if ( (unsigned __int8)sub_101766E0(v2, "func_breakable") || (unsigned __int8)sub_101766E0(v2, "func_breakable_surf") )
//		{
//			if ( (unsigned __int8)sub_101766E0(v2, "func_breakable_surf") && *(_BYTE *)(v2 + 2564) )
//			return 0;
//		}
//		else if ( (*(int (__thiscall **)(int))(*(_DWORD *)v2 + 592))(v2) & 0x10000 )
//		{
//			return 0;
//		}
//		v6 = sub_107FDFD7(v2, 0, &off_10AD5974, &off_10ADB4A4, 0);
//		if ( v6 )
//		{
//			(*(void (**)(void))(*(_DWORD *)v6 + 12))();
//			v7 = a2;
//			if ( v7 <= 0.0 )
//			return 0;
//		}
//		}
//		return 1;
//	}
//
//	char __usercall TraceToExit@<al>(int a1@<edx>, int a2@<ecx>, double a3@<st0>, float a4, float a5, float a6, float a7, float a8, float a9, int a10)
//	{
//	float v10; // xmm6_4@1
//	int v11; // eax@1
//	float v12; // xmm5_4@1
//	float v13; // xmm3_4@1
//	float v14; // xmm4_4@1
//	int v15; // ebx@1
//	int v16; // edi@1
//	float v17; // xmm3_4@2
//	float v18; // xmm1_4@2
//	float v19; // xmm2_4@2
//	float v20; // xmm3_4@2
//	int v21; // eax@4
//	char result; // al@13
//	char v23; // dl@14
//	char v24; // bl@16
//	char v25; // al@17
//	char v26; // al@18
//	float v27; // eax@20
//	float v28; // xmm0_4@21
//	float v29; // xmm2_4@21
//	float v30; // xmm1_4@21
//	float v31; // xmm0_4@21
//	int v32; // eax@24
//	int v33; // ecx@26
//	char v34; // al@27
//	__int64 v35; // [sp+30h] [bp-2Ch]@20
//	float v36; // [sp+38h] [bp-24h]@20
//	float v37; // [sp+3Ch] [bp-20h]@2
//	float v38; // [sp+40h] [bp-1Ch]@2
//	float v39; // [sp+44h] [bp-18h]@2
//	float v40; // [sp+48h] [bp-14h]@2
//	int v41; // [sp+4Ch] [bp-10h]@1
//	int i; // [sp+50h] [bp-Ch]@1
//
//	v10 = a9;
//	v11 = 0;
//	v12 = a8;
//	v13 = 0.0;
//	v14 = a7;
//	v15 = a1;
//	v41 = a1;
//	v16 = a2;
//	for ( i = 0; ; v11 = i )
//	{
//	v17 = v13 + 4.0;
//	v18 = v12 * v17;
//	v19 = v10 * v17;
//	v40 = v17;
//	v20 = a4 + (float)(v14 * v17);
//	*(float *)v16 = v20;
//	*(float *)(v16 + 4) = a5 + v18;
//	*(float *)(v16 + 8) = a6 + v19;
//	v37 = v20 - (float)(v14 * 4.0);
//	v38 = (float)(a5 + v18) - (float)(v12 * 4.0);
//	v39 = (float)(a6 + v19) - (float)(v10 * 4.0);
//	if ( !v11 )
//	i = (**(int (__stdcall ***)(_DWORD, _DWORD, _DWORD))dword_14F83A5C)(v16, 1174421515, 0);
//	v21 = (**(int (__stdcall ***)(_DWORD, _DWORD, _DWORD))dword_14F83A5C)(v16, 1174421515, 0);
//	if ( v21 & 0x600400B && (!(v21 & 0x40000000) || i == v21) )
//	goto LABEL_28;
//	sub_101765E0(1174421515, 0, a10);
//	if ( ((int (__thiscall *)(int (__stdcall ***)(char)))off_10ABEF68[13])(&off_10ABEF68) )
//	sub_10319D90(1174421515, 0, a10, -1032847360);
//	if ( *(_BYTE *)(a10 + 55) == 1 && *(_WORD *)(a10 + 66) < 0 )
//	{
//	sub_1017F920(100679691, *(_DWORD *)(a10 + 76), 0, a10);
//	if ( (unsigned __int8)sub_10185070(a10) && !*(_BYTE *)(a10 + 55) )
//	goto LABEL_13;
//	LABEL_28:
//	v14 = a7;
//	v12 = a8;
//	v10 = a9;
//	goto LABEL_29;
//	}
//	if ( !(unsigned __int8)sub_10185070(a10) || v23 )
//	{
//	if ( *(_DWORD *)(v15 + 76) )
//	{
//	v32 = dword_14A8245C ? (*(int (**)(void))(*(_DWORD *)dword_14A8245C + 28))() : 0;
//	v33 = *(_DWORD *)(v15 + 76);
//	if ( v33 != v32 )
//	{
//	IsBreakableEntity(v33, a3);
//	if ( v34 )
//	{
//	sub_1017E630(v15);
//	result = 1;
//	*(float *)(a10 + 12) = a4 + a7;
//	*(float *)(a10 + 16) = a5 + a8;
//	*(float *)(a10 + 20) = a6 + a9;
//	return result;
//	}
//	}
//	}
//	goto LABEL_28;
//	}
//	v24 = (*(_BYTE *)(v15 + 66) >> 7) & 1;
//	if ( (*(_BYTE *)(a10 + 66) >> 7) & 1 )
//	{
//	IsBreakableEntity(*(_DWORD *)(a10 + 76), a3);
//	if ( v25 )
//	{
//	IsBreakableEntity(*(_DWORD *)(v41 + 76), a3);
//	if ( v26 )
//	{
//	LABEL_13:
//	*(_DWORD *)v16 = *(_DWORD *)(a10 + 12);
//	*(_DWORD *)(v16 + 4) = *(_DWORD *)(a10 + 16);
//	*(_DWORD *)(v16 + 8) = *(_DWORD *)(a10 + 20);
//	return 1;
//	}
//	}
//	if ( !v24 )
//	goto LABEL_28;
//	}
//	v12 = a8;
//	v14 = a7;
//	v27 = *(float *)(a10 + 32);
//	v10 = a9;
//	_mm_storel_epi64((__m128i *)&v35, _mm_loadl_epi64((const __m128i *)(a10 + 24)));
//	v36 = v27;
//	if ( (float)((float)((float)(*((float *)&v35 + 1) * a8) + (float)(*(float *)&v35 * a7)) + (float)(v27 * a9)) <= 1.0 )
//	{
//	result = 1;
//	v28 = *(float *)(a10 + 44) * 4.0;
//	v29 = *(float *)v16 - (float)(a7 * v28);
//	v30 = *(float *)(v16 + 4) - (float)(a8 * v28);
//	v31 = *(float *)(v16 + 8) - (float)(a9 * v28);
//	*(float *)v16 = v29;
//	*(float *)(v16 + 4) = v30;
//	*(float *)(v16 + 8) = v31;
//	return result;
//	}
//	LABEL_29:
//	v13 = v40;
//	if ( v40 > 90.0 )
//	break;
//	v15 = v41;
//	}
//	return 0;
//	}
//
//*/

//
//bool HandleBulletPenetration(weaponinfo_t *wpn_data, FireBulletData &data)
//{
//	surfacedata_t *enter_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
//	int enter_material = enter_surface_data->game.material;
//	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
//
//	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
//	data.current_damage *= pow(wpn_data->flRangeModifier, data.trace_length * 0.002);
//
//	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
//		data.penetrate_count = 0;
//
//	if (data.penetrate_count <= 0)
//		return false;
//	
//	trace_t trace_exit;
//	if (!TraceToExit(data.enter_trace, trace_exit, data.enter_trace.endpos, data.direction))
//	{
//		return false;
//	}
//
//	surfacedata_t *exit_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(trace_exit.surface.surfaceProps);
//	int exit_material = exit_surface_data->game.material;
//
//	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
//	float final_damage_modifier = 0.16f;
//	float combined_penetration_modifier = 0.0f;
//
//	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
//	{
//		combined_penetration_modifier = 3.0f;
//		final_damage_modifier = 0.05f;
//	}
//	else
//	{
//		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
//	}
//
//	if (enter_material == exit_material)
//	{
//		if (exit_material == 87 || exit_material == 85)
//			combined_penetration_modifier = 3.0f;
//		else if (exit_material == 76)
//			combined_penetration_modifier = 2.0f;
//	}
//
//	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
//	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->flPenetration) * 1.25f);
//	float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();
//
//	thickness *= thickness;
//	thickness *= v34;
//	thickness /= 24.0f;
//
//	float lost_damage = fmaxf(0.0f, v35 + thickness);
//
//	if (lost_damage > data.current_damage)
//	{
//		return false;
//	}
//
//	if (lost_damage >= 0.0f)
//	{
//		data.current_damage -= lost_damage;
//	}
//
//	if (data.current_damage < 0.1f)
//	{
//		return false;
//	}
//
//
//	data.src = trace_exit.endpos;
//	data.penetrate_count--;
//
//	return true;
//}

bool Autowall::UseTraceRayHook;

void CalcAngle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = dst - src;
	delta.NormalizeInPlace();
	MathUtils::VectorAngles(delta, angles);
}

void TraceLine(Vector &vecAbsStart, Vector &vecAbsEnd, unsigned int mask, entity* ign, trace_t *tr)
{
	Ray_t ray;

	CTraceFilter filter;
	filter.pSkip = reinterpret_cast<void*>(ign);

	ray.Init(vecAbsStart, vecAbsEnd);

	game_interfaces::Trace->TraceRay(ray, mask, &filter, tr);
}
int LastTakeDamage;
bool IsBreakableEntity(entity* ent)
{
	typedef bool(__thiscall *isBreakbaleEntityFn)(entity*);
	static isBreakbaleEntityFn IsBreakableEntity;

	if (!IsBreakableEntity)
		IsBreakableEntity = (isBreakbaleEntityFn)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 51 56 8B F1 85 F6 74 68 83");

	int& takedamage = *(int*)((DWORD)ent + 0x27C);

	if (!strcmp("CBreakableSurface", ent->GetClientClass()->m_pNetworkName))
	{
		LastTakeDamage = takedamage;
		takedamage = 2; //DAMAGE_YES
	}

	bool res = IsBreakableEntity(ent);

	if (!strcmp("CBreakableSurface", ent->GetClientClass()->m_pNetworkName))
	{
		takedamage = LastTakeDamage;
	}

	return res;
}
#include "LoggerUtils.h"


bool TraceToExit(trace_t& enterTrace, trace_t& exitTrace, Vector startPosition, Vector direction, entity* local)
{
	float currentDistance = 0;
	int firstContents = 0;

	while (currentDistance <= 90.f)
	{
		//Add extra distance to our ray
		currentDistance += 4.f;

		//Multiply the direction vector to the distance so we go outwards, add our position to it.
		Vector start = startPosition + direction * currentDistance;

		if (!firstContents)
			firstContents = game_interfaces::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr); /*0x4600400B*/
		int pointContents = game_interfaces::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(pointContents & MASK_SHOT_HULL) || pointContents & CONTENTS_HITBOX && pointContents != firstContents) /*0x600400B, *0x40000000*/
		{
			//Let's setup our end position by deducting the direction by the extra added distance
			Vector end = start - (direction * 4.f);

			//Let's cast a ray from our start pos to the end pos
			TraceLine(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exitTrace);

			//Let's check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if (exitTrace.startsolid && exitTrace.surface.flags & SURF_HITBOX)
			{
				TraceLine(start, startPosition, MASK_SHOT_HULL, exitTrace.hit_entity, &exitTrace);

				if (exitTrace.DidHit() && !exitTrace.startsolid)
				{
					start = exitTrace.endpos;
					return true;
				}
				continue;
			}

			//Can we hit? Is the wall solid?
			if (exitTrace.DidHit() && !exitTrace.startsolid)
			{
				if (enterTrace.contents & CONTENTS_WINDOW || exitTrace.contents & CONTENTS_WINDOW)
				{
					return true;
				}

				//Is the wall a breakable? If so, let's shoot through it.
				if (IsBreakableEntity(enterTrace.hit_entity) && IsBreakableEntity(exitTrace.hit_entity))
				{
					return true;
				}
			

				if (enterTrace.surface.flags & SURF_NODRAW || !(exitTrace.surface.flags & SURF_NODRAW) && (exitTrace.plane.normal.Dot(direction) <= 1.f))
				{
					float multAmount = exitTrace.fraction * 4.f;
					start -= direction * multAmount;
					return true;
				}
				continue;
			}

			if (!exitTrace.DidHit() || exitTrace.startsolid)
			{
				if (enterTrace.DidHitNonWorldEntity() && IsBreakableEntity(enterTrace.hit_entity))
				{
					exitTrace = enterTrace;
					exitTrace.endpos = start + direction;
					return true;
				}

				continue;
			}
		}
	}
	
	return false;
}
void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;

	ray.Init(vecAbsStart, vecAbsEnd);

	for (int k = 1; k <= game_interfaces::Globals->maxClients; ++k)
	{
		entity*player = game_interfaces::EntityList->GetClientEntity(k);

		if (!player || !(player->m_iHealth() > 0))
			continue;

		if (player->IsDormant())
			continue;

		if (filter && filter->ShouldHitEntity(player, mask) == false)
			continue;

		game_interfaces::Trace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, player, &playerTrace);
		if (playerTrace.fraction < smallestFraction)
		{
			// we shortened the ray - save off the trace
			*tr = playerTrace;
			smallestFraction = playerTrace.fraction;
		}
	}
}

bool isArmored(entity* Entity, int ArmorValue, int Hitgroup)
{
	bool result = false;

	if (ArmorValue > 0)
	{
		switch (Hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			result = true;
			break;
		case HITGROUP_HEAD:
			result = Entity->m_bHasHelmet();
			break;
		}
	}

	return result;
}
void Autowall::ScaleDamage(int Hitgroup, entity*player, float weapon_armor_ratio, float &current_damage)
{
	bool heavArmor = player->m_bHasHeavyArmor();
	int armor = player->m_ArmorValue();

	switch (Hitgroup)
	{
	case HITGROUP_HEAD:

		if (heavArmor)
			current_damage *= (current_damage * 4.f) * 0.5f;
		else
			current_damage *= 4.f;

		break;

	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:

		current_damage *= 1.f;
		break;

	case HITGROUP_STOMACH:

		current_damage *= 1.25f;
		break;

	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:

		current_damage *= 0.75f;
		break;
	}

	if (isArmored(player, armor, Hitgroup))
	{
		float v47 = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_armor_ratio * 0.5f;

		if (heavArmor)
		{
			armor_bonus_ratio = 0.33f;
			armor_ratio = (weapon_armor_ratio * 0.5f) * 0.5f;
			v47 = 0.33f;
		}

		float new_damage = current_damage * armor_ratio;

		if (heavArmor)
			new_damage *= 0.85f;

		if (((current_damage - (current_damage * armor_ratio)) * (v47 * armor_bonus_ratio)) > armor)
			new_damage = current_damage - (armor / armor_bonus_ratio);

		current_damage = new_damage;
	}
}


bool RebuiltHandleBulletPenetration(trace_t &enter_trace, Vector *vecDir, int enter_material, float flPenetrationModifier, float flPenetration, Vector &result, float *bulletdamage, entity* local)
{
	Vector end;
	trace_t trace_exit;
	if (!TraceToExit(enter_trace, trace_exit, enter_trace.endpos, *vecDir, local))
	{
		return true;
	}

	surfacedata_t *exit_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (flPenetrationModifier + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (*bulletdamage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / flPenetration) * 1.25f);
	Vector v = (trace_exit.endpos - enter_trace.endpos);
	float thickness = (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > *bulletdamage)
	{
		return true;
	}

	if (lost_damage >= 0.0f)
		*bulletdamage -= lost_damage;

	if (*bulletdamage < 1.0f)
	{
		return true;
	}

	result = trace_exit.endpos;

	return false;
}

bool HandleBulletPenetration(weaponinfo_t *wpn_data, FireBulletData &data, entity* local)
{
	typedef bool(__thiscall *HandleBulletMem)(entity*, float&, int&, int*, trace_t*, Vector*, float, float, float, int, int, float, int*, Vector*, float, float, float*);
	static HandleBulletMem HandleBulletPenetrationMem;
	if (!HandleBulletPenetrationMem)
	{																							
		HandleBulletPenetrationMem = (HandleBulletMem)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 8B 73 2C");
	}
	else {
		surfacedata_t *enter_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		Vector result;

		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier, enter_surf_damage_mod = enter_surface_data->game.flDamageModifier;
		int unk,enter_surf_material = enter_surface_data->game.material;

		if (HandleBulletPenetrationMem(local, wpn_data->m_flPenetration, enter_surf_material, &unk, &data.enter_trace, &data.direction, 0.f, enter_surf_penetration_mod, enter_surf_damage_mod, 0.f, 0x1002, wpn_data->m_flPenetration, &data.penetrate_count, &data.src, 0.f, 0.f, &data.current_damage))
		{
			return true;
		}
	}
	return false;
}//

bool SimulateFireBullet(baseweapon* pWeapon, FireBulletData &data, entity* local)
{
	Autowall::UseTraceRayHook = false;
	if (pWeapon)
	{
		weaponinfo_t* weaponData = pWeapon->GetCSWpnData();
		if (weaponData)
		{
			data.penetrate_count = 4;
			data.trace_length = 0.0f;
			data.current_damage = (float)weaponData->m_iWeaponDamage;

			while (data.penetrate_count > 0 && data.current_damage > 0.f)
			{
				data.trace_length_remaining = weaponData->m_flWeaponRange - data.trace_length;

				Vector end = data.src + data.direction * data.trace_length_remaining;

				TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local, &data.enter_trace);
				ClipTraceToPlayers(data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace);

				if (data.enter_trace.fraction == 1.0f)
					break;

				if (data.enter_trace.hit_entity->m_iTeamNum() == local->m_iTeamNum()) // If we hit a teammate we cancel the autowall because we wont hit the enemy
					return false;

				if ((data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG) && (data.enter_trace.hitgroup > HITGROUP_GENERIC))
				{
					data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
					data.current_damage *= pow(weaponData->m_flRangeModifier, data.trace_length * 0.002);
					Autowall::ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.hit_entity, weaponData->m_flArmorRatio, data.current_damage);

					return true;
				}

				surfacedata_t *enter_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
				float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
				if (RebuiltHandleBulletPenetration(data.enter_trace, &data.direction, enter_surface_data->game.material, enter_surf_penetration_mod, weaponData->m_flPenetration, data.src, &data.current_damage, local))
				{
					break;
				}
			}
		}
	}

	return false;
}

float Autowall::GetDamage(const Vector& point, entity* local, entity* ent)
{
	if (local)
	{
		if (local->m_iHealth() > 0 && !local->IsDormant())
		{
			baseweapon* weapon = local->GetWeapon();
			if (weapon)
			{
				FireBulletData data;
				data.src = local->GetEyePosition();
				data.filter = CTraceFilter();
				data.filter.pSkip = local;

				Vector angles,delta = point - data.src;  
				MathUtils::VectorAngles(delta, angles); 

				MathUtils::AngleVectors(angles, &data.direction);
				data.direction.NormalizeInPlace();

				if (SimulateFireBullet(weapon, data, local))
				{
					if (data.enter_trace.hit_entity == ent)
					{
						return data.current_damage;
					}
				}
			}
		}
	}
	return 0.f;
}

float Autowall::CanWallbang(Vector point, entity* local, entity* ent)
{
	if (!local)
		return false;
	baseweapon* weapon = local->GetWeapon();
	if (!weapon)
		return false;
	weaponinfo_t *weapon_data = weapon->GetCSWpnData();
	if (!weapon_data)
		return false;

	FireBulletData data;
	data.src = local->GetEyePosition();
	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	Vector angles;
	CalcAngle(data.src, point, angles);
	MathUtils::AngleVectors(angles, &data.direction);
	data.direction.NormalizeInPlace();

	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	data.current_damage = (float)weapon_data->m_iWeaponDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weapon_data->m_flWeaponRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			return false;

		surfacedata_t *enter_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		if (RebuiltHandleBulletPenetration(data.enter_trace, &data.direction, enter_surface_data->game.material, enter_surf_penetration_mod, weapon_data->m_flPenetration, data.src, &data.current_damage, local))
		{
			return data.current_damage;
			break;
		}

		/*surfacedata_t *enter_surface_data = game_interfaces::PhysicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		if (HandleBulletPenetration(weapon_data, data, local))
		{
			return data.current_damage;
			break;
		}*/
	}

	return 0.f;
}