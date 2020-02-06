#pragma once

#include "Vector.h"
#include <Windows.h>

struct model_t;
struct studiohdr_t;
struct vcollide_t;
struct cplane_t;
struct virtualmodel_t;
struct trace_Struct_t;

class QAngle;
class IMaterial;
class CPhysCollide;
class CUtlBuffer;
class IClientRenderable;
class CStudioHdr;

typedef unsigned short MDLHandle_t;

enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,    // has both translucent and opaque sub-partsa
};

class IModelInfo
{
public:
	inline void* GetModel(int Index) {
		return vfunction<void*(__thiscall *)(void*, int)>(this, 1)(this, Index);
	}

	inline int GetModelIndex(const char* Filename) {
		return vfunction<int(__thiscall *)(void*, const char*)>(this, 2)(this, Filename);
	}

	inline const char* GetModelName(const void* Model) {
		return vfunction<const char*(__thiscall *)(void*, const void*)>(this, 3)(this, Model);
	}
	void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
	{
		typedef void *(__thiscall* oGetModelMaterials)(PVOID, const model_t*, int, IMaterial**);
		vfunction<oGetModelMaterials>(this, 19)(this, model, count, ppMaterial);
	}
	studiohdr_t* GetStudiomodel(const model_t* mod)
	{
		typedef studiohdr_t*(__stdcall* oGetStudiomodel)(const model_t*);
		return vfunction<oGetStudiomodel>(this, 32)(mod);
	}
};