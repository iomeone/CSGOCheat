#pragma once

#include "RandomStuff.h"
#include "ModelRender.h"

class IMaterialSystem
{
public:
	IMaterial* FindMaterial(char const* pMaterialName, const char *pTextureGroupName,bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* original)(void*, char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);
		return vfunction<original>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
	ITexture* FindTexture(char const* c, char const* c2, bool b, int i)
	{
		typedef ITexture*(__thiscall* original)(void*, char const*, char const*, bool, int);
		return vfunction<original>(this, 91)(this, c, c2, b, i);
	}
	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall* original)(void*);
		return vfunction<original>(this, 86)(this);
	}
	MaterialHandle_t NextMaterial(MaterialHandle_t handle)
	{
		typedef MaterialHandle_t(__thiscall* original)(void*, MaterialHandle_t);
		return vfunction<original>(this, 87)(this, handle);
	}
	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall* original)(void*);
		return vfunction<original>(this, 88)(this);
	}
	IMaterial* GetMaterial(MaterialHandle_t handle)
	{
		typedef IMaterial*(__thiscall* original)(void*, MaterialHandle_t);
		return vfunction<original>(this, 89)(this, handle);
	}
	IMatRenderContext* GetRenderContext()
	{
		typedef IMatRenderContext* (__thiscall* original)(PVOID);
		return vfunction<original>(this, 115)(this);
	}
};