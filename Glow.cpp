#include "Glow.h"
#include "MemoryUtils.h"
#include "Menu.h"

GlowObjectManager* Glow::GlowObjManager;

void Glow::DrawGlow(entity* ent)
{
	//if (!Glow::GlowObjManager)
	//{
	//	Glow::GlowObjManager = *(GlowObjectManager**)(MemoryUtils::FindBytePattern("client.dll", "A1 ? ? ? ? A8 01 75 4B") + 4);
	//}
	//else {
	//	auto entity_glow_index = *(uint32_t*)((uintptr_t)ent + 41744);

	//	// pointer to the m_GlowObjectDefinitions array
	//	auto glow_object_array = *(GlowObject_t**)Glow::GlowObjManager;

	//	// pointer to the glow object at entity's glow index
	//	auto glow_object = &glow_object_array[entity_glow_index];


	//}
}

void Glow::Draw()
{
	if (!Glow::GlowObjManager)
	{
		Glow::GlowObjManager = *(GlowObjectManager**)(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "0F 11 05 ? ? ? ? 83 C8 01") + 3);
	}
	else {
		entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
		if (local)
		{
			Vector ally_glow_col((float)MenuControls::v_players_esp_ally_glow->GetCustomColor().r() / 255.0f, (float)MenuControls::v_players_esp_ally_glow->GetCustomColor().g() / 255.0f, (float)MenuControls::v_players_esp_ally_glow->GetCustomColor().b() / 255.0f);
			Vector enemy_glow_col((float)MenuControls::v_players_esp_enemy_glow->GetCustomColor().r() / 255.0f, (float)MenuControls::v_players_esp_enemy_glow->GetCustomColor().g() / 255.0f, (float)MenuControls::v_players_esp_enemy_glow->GetCustomColor().b() / 255.0f);

			for (auto i = 0; i < Glow::GlowObjManager->m_GlowObjectDefinitions.Count(); i++)
			{
				GlowObject_t &glow_object = Glow::GlowObjManager->m_GlowObjectDefinitions[i];
				entity* ent = glow_object.m_pEntity;

				if (glow_object.IsUnused() || !ent)
					continue;

				if (!ent->IsPlayer() || ent->IsDormant())
					continue;

				if (ent->m_iTeamNum() == local->m_iTeamNum() && MenuControls::v_players_esp_ally_glow->GetState() || ent->m_iTeamNum() != local->m_iTeamNum() && MenuControls::v_players_esp_enemy_glow->GetState())
				{
					glow_object.m_bRenderWhenOccluded = true;
					glow_object.m_bRenderWhenUnoccluded = false;
					glow_object.m_vGlowColor = local->m_iTeamNum() == ent->m_iTeamNum() ? ally_glow_col : enemy_glow_col;
					glow_object.m_flGlowAlpha = (float)MenuControls::v_players_esp_glow_bloom->GetValue() / 100.f;

					switch (MenuControls::v_players_esp_glow_mode->GetSelectedItem())
					{
					case 0:
						glow_object.m_nGlowStyle = 0;
						glow_object.m_bFullBloomRender = false;
						break;
					case 1:
						glow_object.m_nGlowStyle = 0;
						glow_object.m_bFullBloomRender = true;
						break;
					default:
						break;
					}
				}
			}
		}
	}
}






















//#include "Glow.h"
//
//#include "Entity.h"
//#include "InterfaceManager.h"
//#include "Menu.h"
//
//ITexture* full_frame;
//ITexture* q_size0;
//ITexture* q_size1;
//
//IMaterial* mat_glow;
//IMaterial* mat_halo;
//IMaterial* mat_blur_x;
//IMaterial* mat_blur_y;
//IMaterial* mat_down;
//IMaterial* mat_rim;
//
//ShaderStencilState_t stencilStateDisable;
//ShaderStencilState_t stencilStateWarm;
//ShaderStencilState_t stencilStateLast;
//ShaderStencilState_t stencilState;
//
//IMaterialVar* bloom_variable;
//IMaterialVar* dim_variable;
//
//SpoofedConvar* r_drawothermodels;
//
//void Glow::Glow() {
//	full_frame = game_interfaces::MaterialSystem->FindTexture("_rt_FullFrameFB", "RenderTargets", true, 0);
//	q_size0 = game_interfaces::MaterialSystem->FindTexture("_rt_SmallFB0", "RenderTargets", true, 0);
//	q_size1 = game_interfaces::MaterialSystem->FindTexture("_rt_SmallFB1", "RenderTargets", true, 0);
//
//	mat_glow = game_interfaces::MaterialSystem->FindMaterial("dev/glow_color", "Other textures", true, "glow_color_");
//	mat_rim = game_interfaces::MaterialSystem->FindMaterial("dev/glow_rim3d", "Other textures", true, "glow_rim3d_");
//	mat_down = game_interfaces::MaterialSystem->FindMaterial("dev/glow_downsample", "Other textures", true, "glow_downsample_");
//	mat_blur_x = game_interfaces::MaterialSystem->FindMaterial("dev/glow_blur_x", "Other textures", true, "glow_blur_x_");
//	mat_blur_y = game_interfaces::MaterialSystem->FindMaterial("dev/glow_blur_y", "Other textures", true, "glow_blur_y_");
//	mat_halo = game_interfaces::MaterialSystem->FindMaterial("dev/halo_add_to_screen", "Other textures", true, "halo_add_to_screen_");
//
//	stencilStateDisable.m_bEnable = false;
//	stencilState.m_bEnable = true;
//	stencilState.m_nReferenceValue = 1;
//	stencilState.m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
//	stencilState.m_PassOp = SHADER_STENCILOP_KEEP;
//	stencilState.m_FailOp = SHADER_STENCILOP_KEEP;
//	stencilState.m_ZFailOp = SHADER_STENCILOP_SET_TO_REFERENCE;
//	stencilStateWarm.m_bEnable = true;
//	stencilStateWarm.m_nReferenceValue = 2;
//	stencilStateWarm.m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
//	stencilStateWarm.m_PassOp = SHADER_STENCILOP_SET_TO_REFERENCE;
//	stencilStateWarm.m_FailOp = SHADER_STENCILOP_KEEP;
//	stencilStateWarm.m_ZFailOp = SHADER_STENCILOP_KEEP;
//	stencilStateLast.m_bEnable = true;
//	stencilStateLast.m_nWriteMask = 0x0;
//	stencilStateLast.m_nTestMask = 0x3;
//	stencilStateLast.m_nReferenceValue = 0x0;
//	stencilStateLast.m_CompareFunc = SHADER_STENCILFUNC_EQUAL;
//	stencilStateLast.m_PassOp = SHADER_STENCILOP_KEEP;
//	stencilStateLast.m_FailOp = SHADER_STENCILOP_KEEP;
//	stencilStateLast.m_ZFailOp = SHADER_STENCILOP_KEEP;
//
//	dim_variable = mat_halo->FindVar("$C0_X", NULL, false);
//	bloom_variable = mat_blur_y->FindVar("$bloomamount", NULL, false);
//	r_drawothermodels = new SpoofedConvar(game_interfaces::CVar->FindVar("r_drawothermodels"));
//}
//
//void Glow::Think() {
//	if (!full_frame || !q_size0 || !q_size1 || !mat_glow || !mat_rim || !mat_down || !mat_blur_x || !mat_blur_y || !mat_halo)
//	{
//		Glow::Glow();
//	}
//	else {
//		auto ctx = game_interfaces::MaterialSystem->GetRenderContext();
//
//		Apply(ctx, 1.f);
//	}
//}
//
//void Glow::Render(IMatRenderContext* ctx, float alpha, int x, int y) {
//	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
//	if (local)
//	{
//		ctx->PushRenderTargetAndViewport();
//		
//		float ally_glow_col[4] = { (float)MenuControls::v_players_esp_ally_glow->GetCustomColor().r() / 255.0f, (float)MenuControls::v_players_esp_ally_glow->GetCustomColor().g() / 255.0f, (float)MenuControls::v_players_esp_ally_glow->GetCustomColor().b() / 255.0f , 1.f };
//		float enemy_glow_col[4] = { (float)MenuControls::v_players_esp_enemy_glow->GetCustomColor().r() / 255.0f, (float)MenuControls::v_players_esp_enemy_glow->GetCustomColor().g() / 255.0f, (float)MenuControls::v_players_esp_enemy_glow->GetCustomColor().b() / 255.0f , 1.f };
//		
//		float org_col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//		float org_blend = game_interfaces::RenderView->GetBlend();
//
//		game_interfaces::RenderView->GetColorModulation(org_col);	
//	
//		ctx->SetRenderTarget(full_frame);
//		ctx->Viewport(0, 0, x, y);
//
//		ctx->ClearColor3ub(0, 0, 0);
//		ctx->ClearBuffers(true, false, false);
//
//		game_interfaces::RenderView->SetBlend(alpha);
//
//		RenderableInstance_t instance; instance.m_nAlpha = (uint8_t)(alpha * 255.0f);
//		for (int i = 0; i < game_interfaces::Globals->maxClients; i++) // Player loop
//		{ 
//			entity* ent = game_interfaces::EntityList->GetClientEntity(i); // We get the entity from the entity list
//			if (ent) // If entity is not a nullptr
//			{
//				if (ent->IsPlayer()) // If entity is a valid player
//				{
//					if (!ent->IsDormant() && ent->m_iHealth() > 0) // If entity is valid
//					{
//						if (ent->m_iTeamNum() == local->m_iTeamNum() && MenuControls::v_players_esp_ally_glow->GetState() || ent->m_iTeamNum() != local->m_iTeamNum() && MenuControls::v_players_esp_enemy_glow->GetState())
//						{
//							game_interfaces::ModelRender->ForcedMaterialOverride(mat_glow);
//							game_interfaces::RenderView->SetColorModulation(ent->m_iTeamNum() == local->m_iTeamNum() ? ally_glow_col : enemy_glow_col);
//
//							ent->DrawModel(0x1, instance);
//
//							game_interfaces::ModelRender->ForcedMaterialOverride(NULL);
//							game_interfaces::RenderView->SetColorModulation(org_col);
//						}
//					}
//				}
//			}
//		}
//
//		game_interfaces::RenderView->SetBlend(org_blend);
//
//		ShaderStencilState_t stencilStateDisable;
//		stencilStateDisable.m_bEnable = false;
//		ctx->SetStencilState(stencilStateDisable);
//		ctx->PopRenderTargetAndViewport();
//	}
//}
//
//void Glow::Apply(IMatRenderContext* ctx, float alpha) {
//	static bool first = true;
//
//	float org_blend = game_interfaces::RenderView->GetBlend();
//
//	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
//	if (local)
//	{
//		ctx->BeginPIXEvent(0xFFF5940F, "EntityGlowEffects");
//		ctx->OverrideDepthEnable(true, false, false);
//
//		game_interfaces::ModelRender->ForcedMaterialOverride(mat_glow);
//		game_interfaces::RenderView->SetBlend(0.0f);
//
//		bool glow_being_used = false;
//
//		RenderableInstance_t instance; instance.m_nAlpha = (uint8_t)(alpha * 255.0f);
//		for (int i = 0; i < game_interfaces::Globals->maxClients; i++) // Player loop
//		{ 
//			entity* ent = game_interfaces::EntityList->GetClientEntity(i); // We get the entity from the entity list
//			if (ent) // If entity is not a nullptr
//			{
//				if (ent->IsPlayer()) // If entity is a valid player
//				{
//					if (!ent->IsDormant() && ent->m_iHealth() > 0) // If entity is valid
//					{
//						if (ent->m_iTeamNum() == local->m_iTeamNum() && MenuControls::v_players_esp_ally_glow->GetState() || ent->m_iTeamNum() != local->m_iTeamNum() && MenuControls::v_players_esp_enemy_glow->GetState())
//						{
//							ctx->SetStencilState(stencilState);
//							ent->DrawModel(0x1, instance);
//							ctx->SetStencilState(stencilStateWarm);
//							ent->DrawModel(0x1, instance);
//
//							glow_being_used = true;
//						}
//					}
//				}
//			}
//		}
//
//		ctx->OverrideDepthEnable(false, false, false);
//		ctx->SetStencilState(stencilStateDisable);
//
//		game_interfaces::RenderView->SetBlend(org_blend);
//		game_interfaces::ModelRender->ForcedMaterialOverride(NULL);
//
//		if (!glow_being_used)
//			return;
//
//		int x, y;
//		game_interfaces::Engine->GetScreenSize(x, y);
//
//		ctx->BeginPIXEvent(0xFFF5940F, "RenderGlowModels");
//
//		r_drawothermodels->SetInt(2);
//
//		Render(ctx, alpha, x, y);
//
//		r_drawothermodels->SetInt(1);
//
//		ctx->PushRenderTargetAndViewport();
//
//		int nSrcWidth = x;
//		int nSrcHeight = y;
//		int nViewportX, nViewportY, nViewportWidth, nViewportHeight;
//		ctx->GetViewport(nViewportX, nViewportY, nViewportWidth, nViewportHeight);
//
//		if ((q_size0->GetActualWidth() != (x / 4)) || (q_size0->GetActualHeight() != (y / 4))) 
//		{
//			ctx->SetRenderTarget(q_size0);
//			ctx->Viewport(0, 0, q_size0->GetActualWidth(), q_size0->GetActualHeight());
//			ctx->ClearColor3ub(0, 0, 0);
//			ctx->ClearBuffers(true, false, false);
//		}
//
//		ctx->SetRenderTarget(q_size0);
//		ctx->Viewport(0, 0, x / 4, y / 4);
//
//		ctx->DrawScreenSpaceRectangle(mat_down, 0, 0, nSrcWidth / 4, nSrcHeight / 4, 0, 0, nSrcWidth - 4, nSrcHeight - 4, full_frame->GetActualWidth(), full_frame->GetActualHeight(), nullptr, 0, 0);
//
//		if (first || (q_size1->GetActualWidth() != (x / 4)) || (q_size1->GetActualHeight() != (y / 4))) 
//		{
//			first = false;
//			ctx->SetRenderTarget(q_size1);
//			ctx->Viewport(0, 0, q_size1->GetActualWidth(), q_size1->GetActualHeight());
//			ctx->ClearColor3ub(0, 0, 0);
//			ctx->ClearBuffers(true, false, false);
//		}
//
//		ctx->SetRenderTarget(q_size1);
//		ctx->Viewport(0, 0, x / 4, y / 4);
//
//		ctx->DrawScreenSpaceRectangle(mat_blur_x, 0, 0, nSrcWidth / 4, nSrcHeight / 4, 0, 0, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1, q_size0->GetActualWidth(), q_size0->GetActualHeight(), nullptr, 0, 0);
//
//		ctx->SetRenderTarget(q_size0);
//		ctx->Viewport(0, 0, x / 4, y / 4);
//
//		bloom_variable->SetFloatValue((float)MenuControls::v_players_esp_glow_bloom->GetValue() / 100.f);
//
//		ctx->DrawScreenSpaceRectangle(mat_blur_y, 0, 0, nSrcWidth / 4, nSrcHeight / 4, 0, 0, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1, q_size1->GetActualWidth(), q_size1->GetActualHeight(), nullptr, 0, 0);
//
//		ctx->PopRenderTargetAndViewport();
//
//		dim_variable->SetFloatValue(1.f);
//
//		ctx->SetStencilState(stencilStateLast);
//		ctx->DrawScreenSpaceRectangle(mat_halo, 0, 0, nViewportWidth, nViewportHeight, 0.0f, -0.5f, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1, q_size1->GetActualWidth(), q_size1->GetActualHeight(), nullptr, 0, 0);
//		ctx->SetStencilState(stencilStateDisable);
//	}
//}