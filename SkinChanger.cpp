#include "SkinChanger.h"
#include "InterfaceManager.h"
#include "Menu.h"

std::vector<SkinWeapon> SkinChanger::SkinWeapons;
int SkinChanger::LastWeaponID;
bool SkinChanger::should_update;

void SkinChanger::Update(baseweapon* weapon)
{
	if (weapon)
	{
		WeaponType_t weapon_type = weapon->GetWeaponType();
		if (weapon_type != WEAPON_TYPE_GRENADE && weapon_type != WEAPON_TYPE_OTHER)
		{
			for (SkinWeapon &sw : SkinChanger::SkinWeapons)
			{
				if (sw.WeaponID == weapon->m_iItemDefinitionIndex())
				{
					if (weapon->m_nFallbackPaintKit() != sw.PaintKit || weapon->m_nFallbackSeed() != sw.Seed)
					{

						weapon->m_nFallbackSeed() = sw.Seed;
						weapon->m_nFallbackPaintKit() = sw.PaintKit;
						weapon->m_flFallbackWear() = 0.00000001f;
						weapon->m_iEntityQuality() = 0;

						*(int*)((DWORD)weapon + 0x2FC0) = -1;// m_iItemIDHigh 

						game_interfaces::ClientState->ForceFullUpdate();
					}
				}
			}
		}
	}
}

void SkinChanger::UpdateAll()
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local)
	{
		if (!local->IsDormant() && local->m_iHealth() > 0)
		{
			baseweapon* localweapon = local->GetWeapon();
			if (localweapon)
			{
				SkinChanger::UpdateMenuValue(localweapon);


				// Animation Fix Online
				static int lastSentSeq;
				baseviewmodel* viewmodel = (baseviewmodel*)game_interfaces::EntityList->GetClientEntityFromHandle(local->m_hViewModel());
				if (viewmodel && localweapon)
				{
					if (localweapon->GetWeaponType() == WEAPON_TYPE_KNIFE)
					{
						if (lastSentSeq != viewmodel->m_nSequence())
						{
							lastSentSeq = viewmodel->m_nSequence();
							viewmodel->SendViewModelMatchingSequence(viewmodel->m_nSequence());
						}
					}
				}
			}
		}
	}

	if (SkinChanger::should_update)
	{
		for (int i = 0; i < game_interfaces::EntityList->GetHighestEntityIndex(); i++)
		{
			baseweapon* weap = (baseweapon*)game_interfaces::EntityList->GetClientEntity(i);

			if (!weap)
				continue;

			if (!game_interfaces::EntityList->GetClientEntityFromHandle(weap->m_hOwnerEntity()))
				continue;

			if (local) // Only if we aren't forcing a fullupdate
			{
				if (game_interfaces::EntityList->GetClientEntityFromHandle(weap->m_hOwnerEntity()) != local) // If the weapons is from the local player and not from other players
					continue;
			}

			SkinChanger::UpdateSkins(weap);
		}	
	}
}

void SkinChanger::SetSkin(int weapon, int paint, int seed)
{
	bool found = false;

	for (SkinWeapon &sw : SkinChanger::SkinWeapons)
	{
		if (weapon == sw.WeaponID)
		{
			sw.Seed = seed;
			sw.PaintKit = paint;
			found = true;
		}
	}

	if (!found)
	{
		SkinWeapon new_skin_weapon(weapon, paint, seed);
		SkinChanger::SkinWeapons.push_back(new_skin_weapon);
	}

	
}

void SkinChanger::UpdateSkins(baseweapon* weapon)
{
	if (weapon->GetWeaponType() == WEAPON_TYPE_KNIFE)
	{
		int iBayonet = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		int iButterfly = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		int iFlip = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		int iGut = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		int iKarambit = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		int iM9Bayonet = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		int iHuntsman = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		int iFalchion = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		int iDagger = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		int iBowie = game_interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

		switch (MenuControls::v_skins_knife_model->GetSelectedItem())
		{
		case 0: // Default
			break;
		case 1: // Bayonet
			weapon->m_iWorldModelIndex() = iBayonet + 1;
			weapon->m_iViewModelIndex() = iBayonet;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_BAYONET;
			weapon->SetModelIndex(iBayonet);
			break;
		case 2: // Flip Knife
			weapon->m_iWorldModelIndex() = iFlip + 1;
			weapon->m_iViewModelIndex() = iFlip;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_FLIP;
			weapon->SetModelIndex(iFlip);
			break;
		case 3: // Gut Knife
			weapon->m_iWorldModelIndex() = iGut + 1;
			weapon->m_iViewModelIndex() = iGut;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_GUT;
			weapon->SetModelIndex(iGut);
			break;
		case 4: // Karambit
			weapon->m_iWorldModelIndex() = iKarambit + 1;
			weapon->m_iViewModelIndex() = iKarambit;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_KARAMBIT;
			weapon->SetModelIndex(iKarambit);
			break;
		case 5: // M9 Bayonet
			weapon->m_iWorldModelIndex() = iM9Bayonet + 1;
			weapon->m_iViewModelIndex() = iM9Bayonet;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_M9_BAYONET;
			weapon->SetModelIndex(iM9Bayonet);
			break;
		case 6: // Huntsman
			weapon->m_iWorldModelIndex() = iHuntsman + 1;
			weapon->m_iViewModelIndex() = iHuntsman;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_HUNTSMAN;
			weapon->SetModelIndex(iHuntsman);
			break;
		case 7: // Bowie Knife
			weapon->m_iWorldModelIndex() = iBowie + 1;
			weapon->m_iViewModelIndex() = iBowie;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_SURVIVAL_BOWIE;
			weapon->SetModelIndex(iBowie);
			break;
		case 8: // Butterfly Knife
			weapon->m_iWorldModelIndex() = iButterfly + 1;
			weapon->m_iViewModelIndex() = iButterfly;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_BUTTERFLY;
			weapon->SetModelIndex(iButterfly);
			break;
		case 9: // Shadow Daggers
			weapon->m_iWorldModelIndex() = iDagger + 1;
			weapon->m_iViewModelIndex() = iDagger;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_PUSH;
			weapon->SetModelIndex(iDagger);
			break;
		case 10: // Falchion
			weapon->m_iWorldModelIndex() = iFalchion + 1;
			weapon->m_iViewModelIndex() = iFalchion;
			weapon->m_Item().m_iItemDefinitionIndex() = WEAPON_KNIFE_FALCHION;
			weapon->SetModelIndex(iFalchion);
			break;
		}
	}

	Update(weapon);
}

void SkinChanger::UpdateMenuValue(baseweapon* local_weapon)
{
	if (local_weapon)
	{
		WeaponType_t weapon_type = local_weapon->GetWeaponType();
		if (weapon_type != WEAPON_TYPE_GRENADE && weapon_type != WEAPON_TYPE_OTHER)
		{

			if (LastWeaponID != local_weapon->m_iItemDefinitionIndex())
			{
				LastWeaponID = local_weapon->m_iItemDefinitionIndex();

				bool found = false;
				for (SkinWeapon &sw : SkinChanger::SkinWeapons)
				{
					if (local_weapon->m_iItemDefinitionIndex() == sw.WeaponID)
					{
						found = true;

						if (local_weapon->m_nFallbackPaintKit() != sw.PaintKit)
						{
							SkinChanger::should_update = true;
						}

						MenuControls::v_skins_paint_id->Set(std::to_string(sw.PaintKit));
						MenuControls::v_skins_seed->Set(std::to_string(sw.Seed));
					}
				}
				if (!found)
				{
					MenuControls::v_skins_seed->Set("");
					MenuControls::v_skins_paint_id->Set("");
				}
			}
			else {
				bool paintkit_not_updated = false;
				for (SkinWeapon &sw : SkinChanger::SkinWeapons)
				{
					if (local_weapon->m_nFallbackPaintKit() != sw.PaintKit && sw.WeaponID == local_weapon->m_iItemDefinitionIndex())
					{
						paintkit_not_updated = true;					
					}
				}
				if (paintkit_not_updated)
				{
					SkinChanger::should_update = true;
				}
				else {
					SkinChanger::should_update = false; 
				}

				int menu_value_skin = std::atoi(MenuControls::v_skins_paint_id->GetInput().c_str());
				int menu_value_seed = std::atoi(MenuControls::v_skins_seed->GetInput().c_str());
				SetSkin(local_weapon->m_iItemDefinitionIndex(), menu_value_skin, menu_value_seed);
			}
		}
	}
}