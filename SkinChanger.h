#pragma once

#include "Entity.h"
#include <vector>
#include "NetVars.h"
#include "InterfaceManager.h"

struct SkinWeapon
{
	SkinWeapon(int w_id, int p_kit, int seed)
	{
		this->WeaponID = w_id;
		this->PaintKit = p_kit;
		this->Seed = seed;
	}
	int WeaponID;

	int Seed;
	int PaintKit;
};

namespace SkinChanger
{
	extern int LastWeaponID;

	extern std::vector<SkinWeapon> SkinWeapons;
	extern bool should_update;

	void Update(baseweapon* weapon);
	void SetSkin( int weapon, int paint, int seed);
	void UpdateSkins(baseweapon* weapon);
	void UpdateAll();

	void UpdateMenuValue(baseweapon* local_weapon);

	void RecvProxy_Viewmodel(const CRecvProxyData* pData, void* pStruct, void* pOut);
	void RecvProxy_nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut);
}