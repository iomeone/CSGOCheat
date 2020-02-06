#include "HookManager.h"
#include "RenderUtils.h"

ResetFn hooks::oReset;

HRESULT __stdcall hooks::Reset_Hook(IDirect3DDevice9* Device, D3DPRESENT_PARAMETERS* Parameters)
{
	if (RenderUtils::Normal_font)
	{
		RenderUtils::Normal_font->OnLostDevice();
	}
	if (RenderUtils::Normal_Bold_font)
	{
		RenderUtils::Normal_Bold_font->OnLostDevice();
	}
	if (RenderUtils::Visuals_font)
	{
		RenderUtils::Visuals_font->OnLostDevice();
	}
	if (RenderUtils::BigVisuals_font)
	{
		RenderUtils::BigVisuals_font->OnLostDevice();
	}
	if (RenderUtils::Bold_font)
	{
		RenderUtils::Bold_font->OnLostDevice();
	}
	if (RenderUtils::CheatTitleSprite)
	{
		RenderUtils::CheatTitleSprite->OnLostDevice();
	}
	if (RenderUtils::Menu_Title_font)
	{
		RenderUtils::Menu_Title_font->OnLostDevice();
	}

	HRESULT or = hooks::oReset(Device, Parameters);

	if (RenderUtils::Normal_font)
	{
		RenderUtils::Normal_font->OnResetDevice();
	}
	if (RenderUtils::Normal_Bold_font)
	{
		RenderUtils::Normal_Bold_font->OnResetDevice();
	}
	if (RenderUtils::Visuals_font)
	{
		RenderUtils::Visuals_font->OnResetDevice();
	}
	if (RenderUtils::BigVisuals_font)
	{
		RenderUtils::BigVisuals_font->OnResetDevice();
	}
	if (RenderUtils::Bold_font)
	{
		RenderUtils::Bold_font->OnResetDevice();
	}
	if (RenderUtils::CheatTitleSprite)
	{
		RenderUtils::CheatTitleSprite->OnResetDevice();
	}
	if (RenderUtils::Menu_Title_font)
	{
		RenderUtils::Menu_Title_font->OnResetDevice();
	}
	//if (RenderUtils::ColorPickerSprite)
	//{
	//	RenderUtils::ColorPickerSprite->Release();
	//	RenderUtils::ColorPickerSprite = NULL;
	//}
	//if (RenderUtils::ColorPickerTexture)
	//{
	//	RenderUtils::ColorPickerTexture->Release();
	//	RenderUtils::ColorPickerTexture = NULL;
	//}

	//RenderUtils::InitColorPicker(RenderUtils::directx_device, 201, 201);

	
	return or;
}