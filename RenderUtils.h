#pragma once

#include "VGui.h"
#include "Color.h"
#include "Vector.h"
#include <string>

#include <d3d9.h>

#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9core.h>
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h>

#pragma comment(lib, "d3dx9.lib")
namespace RenderUtils
{
	void LoadFonts();

	extern D3DPRESENT_PARAMETERS* directx_device_params;
	extern LPDIRECT3DDEVICE9 directx_device;
	extern ID3DXFont* Normal_font;
	extern ID3DXFont* Normal_Bold_font;
	extern ID3DXFont* Bold_font;
	extern ID3DXFont* Visuals_font;
	extern ID3DXFont* BigVisuals_font;
	extern ID3DXFont* Menu_Title_font;

	extern LPD3DXSPRITE CheatTitleSprite;
	extern LPDIRECT3DTEXTURE9 CheatTitleTexture;
	
	extern int CheatTitleAnimationCycleCount;
	extern int CheatTitleAnimationCycleCount_Delayer;

	extern int GetTextHeight(std::string text, ID3DXFont* font);
	extern int GetTextLenght(std::string text, ID3DXFont* font);
	extern int GetTextHeight(std::wstring text, ID3DXFont* font);
	extern int GetTextLenght(std::wstring text, ID3DXFont* font);

	extern void RenderText(int x, int y, Color color, LPCSTR text);
	extern void RenderText(int x, int y, Color color, LPWSTR text);
	extern void RenderText(int x, int y, Color color, LPWSTR text, ID3DXFont* font);
	extern void RenderText(int x, int y, Color color, LPCSTR text, ID3DXFont* font);

	extern void RenderRect(float x, float y, float w, float h, Color color);
	extern void RenderOutlineRect(float x, float y, float w, float h, Color color);

	extern void Triangle(Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color);
	extern void TriangleFilled(Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color);
	extern void RotatedTriangle(Vector2D center_pos,float radius,float rotation, Color color);

	extern void RenderVerticalGradientRect(int x, int y, int w, int h, Color color1, Color color2);
	extern void RenderHorizontalGradientRect(int x, int y, int w, int h, Color color1, Color color2);
	extern void RenderLine(float x, float y, float x2, float y2, Color color);
	extern void RenderCircle(int x, int y, float radius, Color color);

	extern void InitCheatTitle(LPDIRECT3DDEVICE9 Device);
	extern void RenderCheatTitle(int x, int y, int w, int h);

	extern D3DCOLOR HSLToRGB(float H, float S, float L);
	extern void RenderColorPicker(int x, int y, int width, int height);

	extern bool WorldToScreen(Vector &in, Vector &out);
	
	extern std::string U16_To_U8(const std::wstring &s);

	extern RECT Viewport();
}