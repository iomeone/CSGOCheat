#pragma once

#include <Windows.h>
#include "RandomStuff.h"

class IClientEntity;
struct model_t;

class IWorldRenderList;

class IRenderView
{
public:
	virtual void                DrawBrushModel(IClientEntity *baseentity, model_t *model, const Vector& origin, const QAngle& angles, bool sort) = 0;
	virtual void                DrawIdentityBrushModel(IWorldRenderList *pList, model_t *model) = 0;
	virtual void                TouchLight(struct dlight_t *light) = 0;
	virtual void                Draw3DDebugOverlays(void) = 0;
	virtual void                SetBlend(float blend) = 0;
	virtual float               GetBlend(void) = 0;
	virtual void                SetColorModulation(float const* blend) = 0;
	inline void                 SetColorModulation(float r, float g, float b) { float color[3] = { r, g, b }; SetColorModulation(color); }
	virtual void                GetColorModulation(float* blend) = 0;
};