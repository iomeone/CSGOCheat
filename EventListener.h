#pragma once

#include "Entity.h"
#include "Vector.h"
#include "ViewRenderBeams.h"
#include "InterfaceManager.h"
#include "Resolver.h"
#include "Visuals.h"
#include "LoggerUtils.h"
#include "Menu.h"


void NewBeam(entity* Entity, Vector Origin, Vector Final)
{
	BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMPOINTS;
	beamInfo.m_pszModelName = "sprites/white.vmt";
	beamInfo.m_nModelIndex = -1; // will be set by CreateBeamPoints if its -1
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = 2.f;
	beamInfo.m_flWidth = 0.5f;
	beamInfo.m_flEndWidth = 0.5f;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = 200.f;
	beamInfo.m_flSpeed = 0.1f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = (float)MenuControls::v_other_projectiles->GetCustomColor().r();
	beamInfo.m_flGreen = (float)MenuControls::v_other_projectiles->GetCustomColor().g();
	beamInfo.m_flBlue = (float)MenuControls::v_other_projectiles->GetCustomColor().b();
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = 0;

	beamInfo.m_vecStart = Origin; 
	beamInfo.m_vecEnd = Final; 

	Beam_t* myBeam = game_interfaces::RenderBeams->CreateBeamPoints(beamInfo);
	if (myBeam)
	{
		game_interfaces::RenderBeams->DrawBeam(myBeam);
	}
}

int GetPlayerByUID(int uid)
{
	for (int i = 0; i < game_interfaces::Globals->maxClients; i++)
	{
		entity* Entity = game_interfaces::EntityList->GetClientEntity(i);
		playerinfo pInfo;
		if (Entity)
		{
			if (game_interfaces::Engine->GetPlayerInfo(i, &pInfo))
			{
				if (pInfo.userID == uid)
				{
					return Entity->GetIndex();
				}
			}
		}
	}
	return -1;
}


class HitListener : public IGameEventListener2
{
public:
	HitListener()
	{
		game_interfaces::GameEvents->AddListener(this, "player_hurt", false);
	};

	~HitListener()
	{
		game_interfaces::GameEvents->RemoveListener(this);
	};

	virtual void FireGameEvent(IGameEvent* Event)
	{
		if (!strcmp("player_hurt", Event->GetName())) 
		{
			int Attackeruserid = Event->GetInt("attacker");
			int Enemyuserid = Event->GetInt("userid");
			int DamageDidToTheEnemy = Event->GetInt("dmg_health");
	
			int AttackerIndex = GetPlayerByUID(Attackeruserid);
			int EnemyIndex = GetPlayerByUID(Enemyuserid);

			if (EnemyIndex > -1 && AttackerIndex > -1)
			{
				if (AttackerIndex == game_interfaces::Engine->GetLocalPlayer() && AttackerIndex != EnemyIndex)
				{
					Visuals::HitMarker_Alpha = 255;
					if (MenuControls::v_other_hitmarker->GetState())
					{
						game_interfaces::Engine->ClientCmd_Unrestricted("play buttons\\arena_switch_press_02", true);
					}

					for (ResolverPlayer &res : Resolver::Resolver_Players)
					{
						if (res.PlayerIndex == EnemyIndex)
						{
							// NoSpread
							res.NoSpread_Brute.R_LastHitTimer = 50;
							res.NoSpread_Brute.R_LastHittedAngle = res.NoSpread_Brute.ResolverAngle;					
						}
					}

					playerinfo pInfo;
					if (game_interfaces::Engine->GetPlayerInfo(EnemyIndex, &pInfo))
					{
						std::string log_string = "[dominance] Hit ";
						log_string += pInfo.name;
						log_string += " [";
						log_string += std::to_string(DamageDidToTheEnemy);
						log_string += " damage done] \n";

						Logger::Message(log_string, Color(181, 249, 114));
					}
				}
				else {
					if (EnemyIndex == game_interfaces::Engine->GetLocalPlayer())
					{
						playerinfo pInfo;
						if (game_interfaces::Engine->GetPlayerInfo(AttackerIndex, &pInfo))
						{
							std::string log_string = "[dominance] ";
							log_string += pInfo.name;
							log_string += " [";
							log_string += std::to_string(DamageDidToTheEnemy);
							log_string += " health lost] \n";

							Logger::Message(log_string, Color(249, 114, 114));
						}
					}
				}
			}
		}
	};

	int GetEventDebugID() override {
		return 42; // 42
	}
};

class BuyListener : public IGameEventListener2 
{
public:
	BuyListener()
	{
		game_interfaces::GameEvents->AddListener(this, "item_purchase", false);
	};

	~BuyListener()
	{
		game_interfaces::GameEvents->RemoveListener(this);
	};

	virtual void FireGameEvent(IGameEvent* Event)
	{
		if (!strcmp("item_purchase", Event->GetName()))
		{
			int PlayerID = Event->GetInt("userid");
			std::string ItemBought = Event->GetString("weapon");

			int PlayerIndex = GetPlayerByUID(PlayerID);

			if (PlayerIndex != -1) 
			{
				if (PlayerIndex != game_interfaces::Engine->GetLocalPlayer())
				{
					playerinfo pInfo;
					if (game_interfaces::Engine->GetPlayerInfo(PlayerIndex, &pInfo))
					{
						std::string log_string = "[dominance] ";
						log_string += pInfo.name;
						log_string += " bought ";
						log_string += ItemBought;
						log_string += " \n";

						Logger::Message(log_string, Color(114, 206, 249));
					}
				}
			}
		}
	};

	int GetEventDebugID() override
	{
		return 42; // 42
	}
};

class BulletImpactListener : public IGameEventListener2 {
public:
	BulletImpactListener() 
	{
		game_interfaces::GameEvents->AddListener(this, "bullet_impact", false);
	};

	~BulletImpactListener()
	{
		game_interfaces::GameEvents->RemoveListener(this);
	};

	virtual void FireGameEvent(IGameEvent* Event) 
	{
		if (!strcmp("bullet_impact", Event->GetName()) && MenuControls::v_other_projectiles->GetState())
		{

			int UserID = Event->GetInt("userid");
			float XImpact = Event->GetFloat("x");
			float YImpact = Event->GetFloat("y");
			float ZImpact = Event->GetFloat("z");

			int Player = GetPlayerByUID(UserID);

			if (Player > -1)
			{
				entity* Entity = game_interfaces::EntityList->GetClientEntity(Player);
				if (Entity) {
					if (Entity->m_iHealth() > 0 && !Entity->IsDormant()) 
					{
						if (game_interfaces::RenderBeams)
						{
							Vector HeadVector;
							if (Entity->GetHitboxPosition(Hitboxes::HITBOX_HEAD, HeadVector))
							{
								NewBeam(Entity, HeadVector, Vector(XImpact, YImpact, ZImpact));
							}
						}
					}
				}
			}
		}
	};

	int GetEventDebugID() override
	{
		return 42; // 42
	}
};