#pragma once

#include <Windows.h>
#include "RandomStuff.h"
#include "Entity.h"
#include "Vector.h"
#include "NetChannelInfo.h"

class IEngine
{
public:
	const char* GetGameDirectory()
	{
		typedef const char* (__thiscall* original)(PVOID);
		return vfunction< original >(this, 36)(this);
	}
	INetChannelInfo* GetNetChannelInfo()
	{
		typedef INetChannelInfo*(__thiscall* original)(PVOID);
		return vfunction< original >(this, 78)(this);
	}
	bool GetPlayerInfo(int ent_num, playerinfo *pinfo)
	{
		typedef bool(__thiscall* original)(PVOID, int, playerinfo*);
		return vfunction< original >(this, 8)(this, ent_num, pinfo);
	}
	bool Connected()
	{
		typedef bool(__thiscall* original)(PVOID);
		return vfunction< original >(this, 27)(this);
	}
	bool InGame()
	{
		typedef bool(__thiscall* original)(PVOID);
		return vfunction< original >(this, 26)(this);
	}
	int GetLocalPlayer()
	{
		typedef int(__thiscall* original)(PVOID);
		if (this)
		{
			if (vfunction< original >(this, 12) != nullptr)
			{
				return vfunction< original >(this, 12)(this);
			}
		}
		return 0;
	}
	const matrix3x4_t& WorldToScreenMatrix()
	{
		typedef const matrix3x4_t& (__thiscall* original)(PVOID);
		return vfunction< original >(this, 37)(this);
	}
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* original)(PVOID, int&, int&);
		return vfunction< original >(this, 5)(this, width, height);
	}
	void ClientCmd_Unrestricted(char const* cmd, bool b)
	{
		typedef void(__thiscall* original)(PVOID, const char*, bool);
		vfunction< original >(this, 114)(this, cmd, b);
	}
	bool IsTakingScreenshot()
	{
		typedef bool(__thiscall* original)(PVOID);
		return vfunction< original >(this, 92)(this);
	}
	void SetViewAngles(Vector& vec)
	{
		typedef void(__thiscall* original)(PVOID, Vector&);
		vfunction< original >(this, 19)(this, vec);
	}
	void GetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* original)(PVOID, Vector&);
		vfunction< original >(this, 18)(this, vAngles);
	}
	int GetMaxClients()
	{
		typedef int(__thiscall* original)(PVOID);
		return vfunction< original >(this, 20)(this);
	}
};