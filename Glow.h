#pragma once

//#include "ModelRender.h"
//#include <stdint.h>
//typedef unsigned __int32 uint32;
//
//namespace Glow
//{
//	void Glow();
//	void Think();
//	void Render(IMatRenderContext* ctx, float alpha, int x, int y);
//	void Apply(IMatRenderContext* ctx, float alpha);
//}

#include "Entity.h"
#include "UtlVector.h"
#include <vector>

struct GlowObject_t 
{
	bool IsUnused() const { return m_nNextFreeSlot != GlowObject_t::ENTRY_IN_USE; }

	entity* m_pEntity;				   //0x0000
	Vector m_vGlowColor;			   //0x0004
	float   m_flGlowAlpha;             //0x0010
	uint8_t pad_0014[4];               //0x0014
	float   m_flSomeFloat;             //0x0018
	uint8_t pad_001C[4];               //0x001C
	float   m_flAnotherFloat;          //0x0020
	bool    m_bRenderWhenOccluded;     //0x0024
	bool    m_bRenderWhenUnoccluded;   //0x0025
	bool    m_bFullBloomRender;        //0x0026
	uint8_t pad_0027[5];               //0x0027
	int32_t m_nGlowStyle;              //0x002C
	int32_t m_nSplitScreenSlot;        //0x0030
	int32_t m_nNextFreeSlot;           //0x0034

	static const int END_OF_FREE_LIST = -1;
	static const int ENTRY_IN_USE = -2;
}; 

class GlowObjectManager
{
public:
	void UnregisterGlowObject(int nGlowObjectHandle)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = NULL;
		m_nFirstFreeSlot = nGlowObjectHandle;
	}

	void SetEntity(int nGlowObjectHandle, entity *pEntity)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = pEntity;
	}

	void SetColor(int nGlowObjectHandle, const Vector &vGlowColor)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_vGlowColor = vGlowColor;
	}

	void SetAlpha(int nGlowObjectHandle, float flAlpha)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_flGlowAlpha = flAlpha;
	}

	void SetRenderFlags(int nGlowObjectHandle, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
	}

	bool IsRenderingWhenOccluded(int nGlowObjectHandle) const
	{
		return m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenOccluded;
	}

	bool IsRenderingWhenUnoccluded(int nGlowObjectHandle) const
	{
		return m_GlowObjectDefinitions[nGlowObjectHandle].m_bRenderWhenUnoccluded;
	}

	bool HasGlowEffect(entity *pEntity) const
	{
		for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i) {
			if (!m_GlowObjectDefinitions[i].IsUnused() && m_GlowObjectDefinitions[i].m_pEntity == pEntity) {
				return true;
			}
		}

		return false;
	}


	CUtlVector<GlowObject_t> m_GlowObjectDefinitions; //0x0000
	int m_nFirstFreeSlot; //0x000C
};

namespace Glow
{
	extern GlowObjectManager* GlowObjManager;
	void DrawGlow(entity* ent);
	void Draw();
}
