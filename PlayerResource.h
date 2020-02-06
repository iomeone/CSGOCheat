#pragma once

#include <Windows.h>
#include "NetVars.h"

class CPlayerResource {};

class C_CSPlayerResource : public CPlayerResource
{
public:
	int m_iPing(int index)
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_PlayerResource", "m_iPing") + index * 4);
	}
	bool m_bFreezePeriod()
	{
		return *(bool*)((DWORD)this + netvars->GetOffset("DT_CSGameRulesProxy", "m_bFreezePeriod"));
	}
};