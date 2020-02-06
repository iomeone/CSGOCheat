#include "SkinChanger.h"
#include "Menu.h"
#include "MathUtils.h"

void SkinChanger::RecvProxy_nSequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	int m_nSequence = pData->m_Value.m_Int;
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());

	if (local)
	{
		baseweapon* weapon = local->GetWeapon();

		if (weapon)
		{
			switch (weapon->m_iItemDefinitionIndex())
			{
			case WEAPON_KNIFE_BUTTERFLY:
				switch (m_nSequence)
				{
				default:
					++m_nSequence;
					break;
				}

				break;
			case WEAPON_KNIFE_FALCHION:
				switch (m_nSequence)
				{
				case 2:
					m_nSequence = 1;
					break;
				case 0:
				case 1:
					break;
				default:
					++m_nSequence;
				}

				break;

			case WEAPON_KNIFE_SURVIVAL_BOWIE:
				switch (m_nSequence)
				{
				case 0:
				case 1:
					break;
				case 2:
					m_nSequence = 1;
					break;
				default:
					--m_nSequence;
					break;
				}

				break;

			case WEAPON_KNIFE_PUSH:
				switch (m_nSequence)
				{
				case 0:
				case 1:
					break;
				case 2:
					m_nSequence = 1;
					break;
				case 10:
				case 11:
				case 12:
					m_nSequence += 3;
					break;
				default:
					m_nSequence += 2;
					break;
				}

				break;
			}
		}
	}

	*(int*)pOut = m_nSequence;
}

void SkinChanger::RecvProxy_Viewmodel(const CRecvProxyData* pData, void* pStruct, void* pOut) // Credits to @n0xius on UC
{
	int iModel = pData->m_Value.m_Int;

	int iCTDefault = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iTDefault = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int iBayonet = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGunGame = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");
	int iGut = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

	if (MenuControls::v_skins_enable->GetState())
	{
		if (iModel == iBayonet
			|| iModel == iButterfly
			|| iModel == iCTDefault
			|| iModel == iTDefault
			|| iModel == iFlip
			|| iModel == iGunGame
			|| iModel == iGut
			|| iModel == iKarambit
			|| iModel == iM9Bayonet
			|| iModel == iHuntsman
			|| iModel == iFalchion
			|| iModel == iBowie)
		{

			switch (MenuControls::v_skins_knife_model->GetSelectedItem())
			{
			case 0: // Default
				break;
			case 1: // Bayonet
				iModel = iBayonet;
				break;
			case 2: // Flip Knife
				iModel = iFlip;
				break;
			case 3: // Gut Knife
				iModel = iGut;
				break;
			case 4: // Karambit
				iModel = iKarambit;
				break;
			case 5: // M9 Bayonet
				iModel = iM9Bayonet;
				break;
			case 6: // Huntsman
				iModel = iHuntsman;
				break;
			case 7: // Bowie Knife
				iModel = iBowie;
				break;
			case 8: // Butterfly Knife
				iModel = iButterfly;
				break;
			case 9: // Shadow Daggers
				iModel = iDagger;
				break;
			case 10: // Falchion
				iModel = iFalchion;
				break;
			}
		}
	}

	*(int*)pOut = iModel;
}
