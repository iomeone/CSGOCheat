#include "Entity.h"

#include "InterfaceManager.h"
#include "MemoryUtils.h"
#include "RandomStuff.h"
#include "EngineTrace.h"
#include "LagCompensation.h"

using LookupPoseParameterFn = int(__stdcall*)(entity* me, const char* str);
LookupPoseParameterFn LookupPoseParameterGame;

int entity::GetSequenceActivity(int sequence)
{
	if (this->GetModel() != nullptr)
	{
		studiohdr_t* hdr = game_interfaces::ModelInfo->GetStudiomodel(this->GetModel());
		if (hdr)
		{
			typedef int(__fastcall* fnSequenceActivity)(void*, studiohdr_t*, int);
			static auto GetSequenceActivity = (fnSequenceActivity)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 7D 08 FF 56 8B F1 74");
			if (GetSequenceActivity)
			{
				return GetSequenceActivity(this, hdr, sequence);
			}
		}
	}
	return -1;
}

int entity::LookupPoseParameter(char *name)
{
	if (!LookupPoseParameterGame)
	{
		LookupPoseParameterGame = (LookupPoseParameterFn)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 57 8B 7D 08 85 FF 75 08 83 C8 FF 5F 5D C2 08 00");
	}

	return LookupPoseParameterGame(this, name);
}

void entity::SetObserverMode(int i)
{
	*(int*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_iObserverMode")) = i;
}

void entity::SetAbsAngles(Vector angles)
{
	typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
	static SetAngleFn SetPlayerAngle = (SetAngleFn)((DWORD)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
	if (SetPlayerAngle)
	{
		SetPlayerAngle(this, angles);
	}
}

void entity::SetAbsOrigin(Vector origin)
{
	typedef void(__thiscall* SetAbsOriginFn)(void*, const Vector &origin);
	static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)((DWORD)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D"));
	if (SetAbsOrigin)
	{
		SetAbsOrigin(this, origin);
	}
}

void entity::InvalidateOcclusion()
{
	*(int*)((uintptr_t)this + 0xA30) = game_interfaces::Globals->framecount; // Clear: Occlusion checks
	*(int*)((uintptr_t)this + 0xA28) = 0; // Clear: Occlusion flags
}

void entity::DisableInterpolation()
{
	varmapping_t* varmap = varmapping();
	if (varmap)
	{
		if (varmap->InterpolatedEntries > 0)
		{
			varmap->InterpolatedEntries = 0;
		}
	}
}

bool entity::IsPlayer()
{
	if (this != nullptr)
	{
		return (this->GetClientClass()->m_ClassID == (int)ClassID::CCSPlayer);
	}
	return false;
}

Vector entity::GetEyePosition()
{
	return(m_vecOrigin() + m_vecViewOffset());
}

bool entity::IsVisible(entity* local)
{
	for (int i = 0; i < this->GetNumHitBoxes(); i++)
	{
		Vector vec;
		if (this->GetHitboxPosition(i, vec))
		{
			if (IsVisiblePosition(vec, local))
			{
				return true;
			}
		}
	}
	return false;
}

animationstate* entity::GetAnimationState()
{
	static int basePlayerAnimStateOffset = 0x3900;
	return *(animationstate**)((DWORD)this + basePlayerAnimStateOffset);
}

baseweapon* entity::GetWeapon() // Crashing
{
	typedef baseweapon*(__thiscall * original)(void*);
	return vfunction<original>(this, 267)(this);
}

weaponinfo_t* baseweapon::GetCSWpnData()
{
	if (this)
	{
		entity* owner = game_interfaces::EntityList->GetClientEntityFromHandle(this->m_hOwnerEntity());
		if (owner)
		{
			if (owner->m_iHealth() > 0)
			{
				if (!owner->IsDormant())
				{
					typedef weaponinfo_t*(__thiscall* original)(void*);
					weaponinfo_t* weapon_info = vfunction<original>(this, 456)(this);
					if (weapon_info)
					{
						return weapon_info;
					}
				}
			}
		}
	}
	return nullptr;
}
matrix3x4_t* entity::GetBonePositionMatrix(int bone)
{
	matrix3x4_t boneMatrix[128];
	if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
	{
		return boneMatrix;
	}
	return nullptr;
}

Vector entity::GetBonePosition(int bone)
{
	matrix3x4_t boneMatrix[128];
	if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
	{
		return Vector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);
	}
	return Vector(0, 0, 0);
}

bool entity::IsVisibleHitBoxPosition(Vector pos, int hitbox, entity* local) {
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter; filter.pSkip = this;

	ray.Init(local->GetEyePosition(), pos);
	game_interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	if (tr.IsVisible() && tr.hitbox == hitbox || tr.hit_entity == this && tr.hitbox == hitbox)
	{
		return true;
	}

	return false;
}

#include "LoggerUtils.h"

bool entity::IsVisiblePosition(Vector pos, entity* local) {
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter; filter.pSkip = local;

	ray.Init(local->GetEyePosition(), pos);
	game_interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	if (tr.IsVisible() || tr.hit_entity == this)
	{
		return true;
	}

	return false;
}

int entity::GetPing()
{
	if (this)
	{
		int ping = (*game_interfaces::PlayerResources)->m_iPing(this->GetIndex());
		return ping;
	}
	return 0;
}


float entity::GetOldSimulationTime()
{
	DWORD offset = netvars->GetOffset("DT_BaseEntity", "m_flSimulationTime") + 0x004;
	return *(float*)((DWORD)this + offset);
}


float entity::GetServerSimulationTime() // Credits to @Paw on UC
{
	// Calculate lerp remainder
	float lerp_remainder = fmodf(LC::GetLerpTime(), game_interfaces::Globals->interval_per_tick);

	// Get real simulation time and calculate interp fraction
	float real_target_time = this->m_flSimulationTime();
	float frac = 0.f;
	if (lerp_remainder > 0.f)
	{
		real_target_time += game_interfaces::Globals->interval_per_tick - lerp_remainder;
		// Trigger float inaccuracies (if there is any) and get same frac as the server
		frac = (real_target_time - this->m_flSimulationTime()) / game_interfaces::Globals->interval_per_tick;
	}

	return real_target_time;
}

Vector entity::GetRagdollPos()
{
	DWORD offset = netvars->GetOffset("DT_BaseAnimating", "m_bClientSideRagdoll") - 0x004;
	ragdoll* rag = (ragdoll*)((DWORD)this + offset);
	if (rag)
	{
		return rag->m_ragPos();
	}
	return Vector(0, 0, 0);
}

bool entity::GetHitboxPosition(int Hitbox, Vector &end)
{
	if (Hitbox < 0 || Hitbox > this->GetNumHitBoxes())
		return false;

	if (!this)
		return false;

	if (this->GetAbsOrigin().Length() < 1)
		return false;

	if (!(this->m_iHealth() > 0))
		return false;

	if (!this->GetClientRenderable())
		return false;

	if (!this->GetClientRenderable()->GetIClientUnknown())
		return false;

	this->InvalidateOcclusion();

	matrix3x4_t matrix[128];
	if (this->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
	{
		studiohdr_t* hdr = game_interfaces::ModelInfo->GetStudiomodel(this->GetModel());
		if (hdr)
		{
			mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
			if (set)
			{
				mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);
				if (hitbox)
				{
					if (hitbox->bbmax.IsValid() && hitbox->bbmin.IsValid())
					{
						Vector vMin, vMax, vCenter, sCenter;
						hitbox->bbmin.z = 0;
						hitbox->bbmax.z = 0;
						if (hitbox->bbmin.Length() > 0 && hitbox->bbmax.Length() > 0 && hitbox->bone >= 0) {
							MathUtils::VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
							MathUtils::VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
							if (vMin.Length() > 0 && vMax.Length() > 0) {
								end = (vMin + vMax) * 0.5f;
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}
CBoneAccessor* entity::m_BoneAccessor()
{
	return (CBoneAccessor*)((uintptr_t)this + 0x26A8);
}

CStudioHdr *entity::GetModelPtr()
{
	return *(CStudioHdr**)((uintptr_t)this + 0x294C);
}
bool entity::RebuiltSetupBones(matrix3x4_t *boneOut, int boneMask)
{
	entity* target = this;

	CStudioHdr *hdr = this->GetModelPtr();

	if (!hdr)
		return false;

	auto accessor = target->m_BoneAccessor();

	if (!accessor)
		return false;

	matrix3x4_t* backup_matrix = accessor->GetBoneArrayForWrite();

	if (!backup_matrix)
		return false;

	int oldReadableBones = *(int*)((DWORD)target->GetClientRenderable() + 9880);

	int bonesMaskNeedRecalc = boneMask | oldReadableBones;

	auto oldBones = accessor->GetBoneArrayForWrite();
	matrix3x4_t baseMatrix;
	MathUtils::AngleMatrix(target->GetAbsAngles(), target->GetAbsOrigin(), baseMatrix);
	target->m_nEffects() |= EF_NOINTERP;
	//g_bPVSFix = true;

	DWORD* m_pIk = *(DWORD**)((DWORD)target->GetClientRenderable() + 0x266C);
	if (m_pIk)
	{
		/*ClearTargets(m_pIk);

		typedef void(__thiscall *Initfn)(void* aids, CStudioHdr*, Vector&, Vector&, float, int, int);
		static Initfn Init = (Initfn)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 EC 08 8B 45 08 56 57 8B F9 82");
		*/
		//Init(m_pIk, hdr, target->GetAbsAngles(), target->GetAbsOrigin(), game_interfaces::Globals->curtime, game_interfaces::Globals->framecount, bonesMaskNeedRecalc);
	}

	Vector *pos = (Vector*)(game_interfaces::MemAlloc->Alloc(sizeof(Vector[128])));
	Quaternion *q = (Quaternion*)(game_interfaces::MemAlloc->Alloc(sizeof(Quaternion[128])));
	std::memset(pos, 0xFF, sizeof(pos));
	std::memset(q, 0xFF, sizeof(q));

	accessor->SetBoneArrayForWrite(boneOut);

	target->StandardBlendingRules(hdr, pos, q, game_interfaces::Globals->curtime, boneMask);

	byte *boneComputed = (byte*)(game_interfaces::MemAlloc->Alloc(sizeof(byte[0x20])));
	std::memset(boneComputed, 0, sizeof(byte[0x20]));

	if (m_pIk)
	{

		/*target->UpdateIKLocks(game_interfaces::Globals->curtime);

		typedef void(__thiscall *fn)(void* aids, Vector*, Quaternion*, void*, byte*);
		typedef void(__thiscall *pSolveDependencies) (DWORD *m_pIk, DWORD UNKNOWN, Vector *pos, Quaternion* q, matrix3x4_t* bonearray, byte *computed);

		static fn UpdateTargets = (fn)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 E4 F0 81 EC ? ? ? ? 34");
		static pSolveDependencies SolveDependencies = (pSolveDependencies)MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 E4 F0 81 EC ? ? ? ? 8B 82");


		if (UpdateTargets)
		{
		UpdateTargets(m_pIk, pos, q, boneOut, boneComputed);
		}

		target->CalculateIKLocks(game_interfaces::Globals->curtime);

		if (SolveDependencies)
		{
		SolveDependencies(m_pIk, 0, pos, q, boneOut, boneComputed);
		}*/
	}
	target->BuildTransformations(hdr, pos, q, baseMatrix, boneMask, boneComputed);

	accessor->SetBoneArrayForWrite(backup_matrix);

	return true;
}