#include "HookManager.h"
#include "InterfaceManager.h"
#include "Entity.h"
#include "Menu.h"
#include "Visuals.h"
#include "MiscFunctions.h"

DrawModelExecuteFn hooks::oDrawModelExecute;

std::vector<const char*> SmokeMaterialsToHide =
{
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};

void RenderHitboxes(entity* ent, Color c)
{
	matrix3x4_t matrix[128];
	if (ent->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
	{
		ESP::DrawHitBoxes(ent, c, 0.05f, matrix);
	}
}

#include "Glow.h"

void __fastcall hooks::DrawModelExecute_Hook(void* thisptr, void* edx, IMatRenderContext* ctx, DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (game_interfaces::Engine->InGame() && game_interfaces::Engine->Connected())
	{
		entity* local = (entity*)game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
		entity* model_ent = (entity*)game_interfaces::EntityList->GetClientEntity(pInfo.entity_index);
		std::string model_name = game_interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
		if (model_name.find("models/player") != std::string::npos)
		{
			if (model_ent && local)
			{
				if (model_ent == local || local->m_iTeamNum() != model_ent->m_iTeamNum() && MenuControls::v_players_esp_enemy_enable->GetState() || local->m_iTeamNum() == model_ent->m_iTeamNum() && MenuControls::v_players_esp_ally_enable->GetState())
				{
					if (MenuControls::v_players_esp_enable->GetState())
					{
						switch (MenuControls::v_other_nosmoke->GetSelectedItem())
						{
						case 0: // Disabled
							for (const char* material : SmokeMaterialsToHide)
							{
								IMaterial* mat = game_interfaces::MaterialSystem->FindMaterial(material, TEXTURE_GROUP_OTHER);
								mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
							}
							break;
						case 1: // Wireframe
							for (const char* material : SmokeMaterialsToHide)
							{
								IMaterial* mat = game_interfaces::MaterialSystem->FindMaterial(material, TEXTURE_GROUP_OTHER);
								if (!strcmp(material, "particle/vistasmokev1/vistasmokev1_smokegrenade"))
								{
									mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
									mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
								}
								else {
									mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
									mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
								}
							}
							break;
						case 2: // Full
							for (const char* material : SmokeMaterialsToHide)
							{
								IMaterial* mat = game_interfaces::MaterialSystem->FindMaterial(material, TEXTURE_GROUP_OTHER);
								mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
							}
							break;
						}

						if (model_ent == local && local->m_bIsScoped())
						{
							game_interfaces::RenderView->SetBlend(0.15f);
						}
						if (model_ent->m_iTeamNum() == local->m_iTeamNum() && MenuControls::v_players_esp_ally_chams->GetState() || model_ent->m_iTeamNum() != local->m_iTeamNum() && MenuControls::v_players_esp_enemy_chams->GetState())
						{
							switch (MenuControls::v_players_esp_chams_mode->GetSelectedItem())
							{
							case 2: // Wireframe
								game_interfaces::RenderView->SetBlend(0.1f);
								break;
							case 3: // Transparent
								game_interfaces::RenderView->SetBlend(0.15f);
								break;
							case 4: // Hitboxes
								game_interfaces::RenderView->SetBlend(0.1f);
							default:
								break;
							}
						}
					}
				}
			}
			Misc_Functions::UpdateNightMode();
		}
		hooks::oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		game_interfaces::ModelRender->ForcedMaterialOverride(nullptr);
	}
	hooks::oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
}