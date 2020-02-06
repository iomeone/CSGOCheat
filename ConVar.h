#pragma once

#include "RandomStuff.h"

class ConVar
{
public:
	float GetFloat()
	{
		typedef float(__thiscall *original)(void*);
		return vfunction<original>(this, 12)(this);
	}

	int GetInt()
	{
		typedef int(__thiscall *original)(void*);
		return vfunction<original>(this, 13)(this);
	}

	void SetValue(const char* value)
	{
		typedef void(__thiscall *original)(void*, const char*);
		vfunction<original>(this, 14)(this, value);
	}

	void SetValue(float value)
	{
		typedef void(__thiscall *original)(void*, float);
		vfunction<original>(this, 15)(this, value);
	}

	void SetValue(int value)
	{
		typedef void(__thiscall *original)(void*, int);
		vfunction<original>(this, 16)(this, value);
	}

	char unk0[0x4];
	ConVar *pNext;
	int bRegistered;
	char *pszName;
	char *pszHelpString;
	int nFlags;
	char unk1[0x4];
	ConVar *pParent;
	char *pszDefaultValue;
	char *strString;
	int StringLength;
	float fValue;
	int nValue;
	int bHasMin;
	float fMinVal;
	int bHasMax;
	float fMaxVal;
	void *fnChangeCallback;
};



class SpoofedConvar {
public:
	SpoofedConvar(const char* szCVar);
	SpoofedConvar(ConVar* pCVar);

	~SpoofedConvar();

	bool		IsSpoofed();
	void		Spoof();

	void		SetFlags(int flags);

	void		SetInt(int iValue);
	void		SetFloat(float flValue);
	void		SetString(const char* szValue);

	int			GetInt();
	float		GetFloat();

private:
	ConVar* m_pOriginalCVar = nullptr;
	ConVar* m_pDummyCVar = nullptr;

	char m_szDummyName[128];
	char m_szDummyValue[128];
	char m_szOriginalName[128];
	char m_szOriginalValue[128];
	int m_iOriginalFlags;
};
