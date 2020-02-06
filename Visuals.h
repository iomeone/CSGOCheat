#pragma once

#include "MaterialUtils.h"

#include <vector>
#include "Color.h"
#include "Vector.h"
#include "Entity.h"

struct ESP_CollisionBox
{
	ESP_CollisionBox(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
	int x, y, w, h;
};

namespace ESP
{
	ESP_CollisionBox GetRenderPoint(entity* ent);

	void DrawBox(float BoxY, float BoxX, float BoxW, float BoxH, entity* ent, entity* local);
	void DrawHealthBar(float BoxY, float BoxX, float BoxW, float BoxH, entity* ent, entity* local);
	void DrawOther(float BoxY, float BoxX, float BoxW, float BoxH, entity* ent, entity* local);

	void DrawHitBoxes(entity* ent, Color col, float duration, matrix3x4_t* matrix);
}

namespace Materials
{
	extern IMaterial* material_normal;
	extern IMaterial* material_normal_ignorez;

	extern IMaterial* material_flat;
	extern IMaterial* material_flat_ignorez;

	extern IMaterial* material_wireframe;
	extern IMaterial* material_wireframe_ignorez;
}

struct ESPInfo
{
	ESPInfo(int indx, std::string txt)
	{
		this->index = indx;
		this->text = txt;
	}
	int index;
	std::string text;
};

namespace Visuals
{
	extern int ESPAlpha[100];
	extern int HitMarker_Alpha;
	extern std::vector<ESPInfo> ESPInfo_List;

	void DrawOnScreenRadar(entity* local, entity* ent);
	void PaintTraverse_Render();
	void SceneEnd_Render();
	std::vector<std::string> GetEspInfo(int idx);
}