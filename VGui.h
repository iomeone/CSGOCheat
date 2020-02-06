#pragma once

#include <Windows.h>
#include "RandomStuff.h"
#include "Color.h"

//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class KeyValues;
struct DmxElementUnpackStructure_t;
class CDmxElement;

// adds to the font
enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2,
};

#include "Vector2D.h"

struct Vertex_t
{
	Vector2D	m_Position;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
};

typedef  Vertex_t FontVertex_t;

namespace vgui
{
	// handle to an internal vgui panel
	// this is the only handle to a panel that is valid across dll boundaries
	typedef unsigned int VPANEL;

	// handles to vgui objects
	// NULL values signify an invalid value
	typedef unsigned long HScheme;
	typedef unsigned long HTexture;
	typedef unsigned long HCursor;
	typedef unsigned long HPanel;
	const HPanel INVALID_PANEL = 0xffffffff;
	typedef unsigned long HFont;
	const HFont INVALID_FONT = 0; // the value of an invalid font handle

	class SurfacePlat;
	class IClientPanel;

	//-----------------------------------------------------------------------------
	// Purpose: interface from Client panels -> vgui panels
	//-----------------------------------------------------------------------------
	class IPanel
	{
	public:
		// gets names of the object (for debugging purposes)
		const char *GetName(VPANEL vguiPanel)
		{
			typedef const char* (__thiscall* original)(PVOID, unsigned int);
			return vfunction<original>(this, 36)(this, vguiPanel);
		}
		void SetMouseInputEnabled(unsigned int panel, bool state)
		{
			return vfunction<void (__thiscall *) (PVOID, int, bool)>(this, 32) (this, panel, state);
		}
	};
	//-----------------------------------------------------------------------------
	// Purpose: Wraps contextless windows system functions
	//-----------------------------------------------------------------------------
	class ISurface
	{
	public:
		void DrawSetColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* original)(PVOID, int, int, int, int);
			vfunction< original >(this, 15)(this, r, g, b, a);
		}
		void DrawSetColor(Color col)
		{
			typedef void(__thiscall* original)(PVOID, Color);
			return vfunction< original >(this, 14)(this, col);
		}
		void DrawFilledRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* original)(PVOID, int, int, int, int);
			vfunction< original >(this, 16)(this, x0, y0, x1, y1);
		}
		void DrawOutlinedRect(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* original)(PVOID, int, int, int, int);
			vfunction< original >(this, 18)(this, x0, y0, x1, y1);
		}
		void DrawLine(int x0, int y0, int x1, int y1)
		{
			typedef void(__thiscall* original)(PVOID, int, int, int, int);
			vfunction< original >(this, 19)(this, x0, y0, x1, y1);
		}
		void DrawSetTextFont(HFont font)
		{
			typedef void(__thiscall* original)(PVOID, unsigned long);
			vfunction< original >(this, 23)(this, font);
		}
		void DrawSetTextColor(int r, int g, int b, int a)
		{
			typedef void(__thiscall* original)(PVOID, int, int, int, int);
			vfunction< original >(this, 25)(this, r, g, b, a);
		}
		void DrawSetTextColor(Color col)
		{
			typedef void(__thiscall* original)(PVOID, Color);
			return vfunction< original >(this, 24)(this, col);
		}
		void DrawSetTextPos(int x, int y)
		{
			typedef void(__thiscall* original)(PVOID, int, int);
			vfunction< original >(this, 26)(this, x, y);
		}
		void DrawPrintText(const wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
		{
			typedef void(__thiscall* original)(PVOID, const wchar_t*, int, int);
			vfunction< original >(this, 28)(this, text, textLen, 0);
																			}
		void DrawPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
		{
			typedef void(__thiscall* original)(PVOID, int, FontVertex_t*, bool);
			vfunction< original >(this, 106)(this, vtxCount, vtx, bClipVertices);
		}
		void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
		{
			typedef void(__thiscall* original)(PVOID, unsigned long, const wchar_t*, int&, int&);
			vfunction< original >(this, 79)(this, font, text, wide, tall);
		}
		vgui::HFont FontCreate()
		{
			typedef unsigned int(__thiscall* original)(PVOID);
			return vfunction< original >(this, 71)(this);
		}
		void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
		{
			typedef void(__thiscall* original)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
			vfunction< original >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
		}
		void UnLockCursor()
		{
			typedef void(__thiscall* original)(void*);
			vfunction< original >(this, 66)(this);
		}
	};

} // namespace vgui
