#include <iostream>

#include "LagCompensation.h"
#include "InterfaceManager.h"
#include "RenderUtils.h"
#include "NetChannelInfo.h"
#include "MaterialUtils.h"
#include "Menu.h"
#include "Visuals.h"

#include "LoggerUtils.h"

#include "Aimbot.h"

#include "AutoWall.h"

/***
*      ____             _ _______             _
*     |  _ \           | |__   __|           | |
*     | |_) | __ _  ___| | _| |_ __ __ _  ___| | __
*     |  _ < / _` |/ __| |/ / | '__/ _` |/ __| |/ /
*     | |_) | (_| | (__|   <| | | | (_| | (__|   <
*     |____/ \__,_|\___|_|\_\_|_|  \__,_|\___|_|\_\
*
*
*/

std::vector<BackTrackPlayer> LC::BackTrack_Players;
float LC::MaxTicks;

float LC::GetPing()
{
	float ping = 0.f;
	INetChannelInfo* net_channel = game_interfaces::Engine->GetNetChannelInfo();
	if (net_channel)
	{
		ping = net_channel->GetLatency(FLOW_INCOMING) + net_channel->GetLatency(FLOW_OUTGOING);
	}
	return ping;
}

bool LC::IsVisiblePoint(entity* Local, Vector Point)
{
	Ray_t ray;
	ray.Init(Local->GetEyePosition(), Point);
	CTraceFilter filter; filter.pSkip = Local;
	trace_t trace;

	game_interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &trace);

	if (trace.IsVisible())
	{
		return true;
	}
	return false;
}

bool LC::TargetValid(entity* ent) // If entity is valid
{
	entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
	if (local && ent)
	{
		if (local->m_iHealth() > 0)
		{
			if (ent->m_iTeamNum() != local->m_iTeamNum() && ent->m_iHealth() > 0 && !ent->IsDormant() && ent->IsPlayer())
			{
				if (ent->GetIndex() != game_interfaces::Engine->GetLocalPlayer())
				{
					return true;
				}
			}
		}
	}
	return false;
}

float LC::GetLerpTime()
{
	auto updateRate = game_interfaces::CVar->FindVar("cl_updaterate")->GetInt();
	auto interpRatio = game_interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();
	auto minInterpRatio = game_interfaces::CVar->FindVar("sv_client_min_interp_ratio")->GetFloat();
	auto maxInterpRatio = game_interfaces::CVar->FindVar("sv_client_max_interp_ratio")->GetFloat();
	auto minUpdateRate = game_interfaces::CVar->FindVar("sv_minupdaterate")->GetInt();
	auto maxUpdateRate = game_interfaces::CVar->FindVar("sv_maxupdaterate")->GetInt();

	auto clampedUpdateRate = clamp(updateRate, minUpdateRate, maxUpdateRate);
	auto clampedInterpRatio = clamp(interpRatio, minInterpRatio, maxInterpRatio);

	auto lerp = clampedInterpRatio / clampedUpdateRate;

	if (lerp <= game_interfaces::CVar->FindVar("cl_interp")->GetFloat())
		lerp = game_interfaces::CVar->FindVar("cl_interp")->GetFloat();

	return lerp;
}

bool LC::TickIsValid(float simulation_time) // UC @polak
{
	ConVar* sv_maxunlag = game_interfaces::CVar->FindVar("sv_maxunlag");
	if (!sv_maxunlag)
		return false;

	float correct = 0;
	correct += LC::GetLerpTime(); correct += LC::GetPing();
	correct = clamp(correct, 0.f, sv_maxunlag->GetFloat());

	float deltaTime = correct - (game_interfaces::Globals->curtime - simulation_time);

	//std::cout << "simulation_time : " << simulation_time << " deltaTime: " << fabsf(deltaTime) << std::endl;

	return !(fabsf(deltaTime) > 0.2f);
}

void LC::DrawBackTrack(entity* ent, entity* local)
{
	if (TargetValid(ent))
	{
		switch (MenuControls::v_players_esp_drawbacktrack->GetSelectedItem())
		{
		case 0: // Disabled
			break;
		case 1: // Square
			for (BackTrackPlayer &btp : LC::BackTrack_Players)
			{
				if (btp.PlayerIndex == ent->GetIndex())
				{
					if (btp.BestRecord.ID >= 0 && btp.BestRecord.head_vector.Length() > 0)
					{
						if (LC::IsVisiblePoint(local, btp.BestRecord.head_vector))
						{
							Vector W2S;
							if (RenderUtils::WorldToScreen(btp.BestRecord.head_vector, W2S))
							{
								RenderUtils::RenderRect(W2S.x - 2, W2S.y - 2, 5, 5, Color::White());
							}
						}
					}
				}
			}
			break;
		case 2: // Chams
			for (BackTrackPlayer &btp : LC::BackTrack_Players)
			{
				if (btp.PlayerIndex == ent->GetIndex())
				{
					if (btp.BestRecord.ID >= 0 && btp.BestRecord.head_vector.Length() > 0)
					{
						RenderableInstance_t instance;
						instance.m_nAlpha = (uint8_t)(255.0f);
						matrix3x4_t matrix[128];

						Vector backup_origin = ent->GetAbsOrigin();
						Vector backup_angles = ent->GetAbsAngles();

						IMaterial* chams_material = Materials::material_normal;
		
						float Enemy_ChamsColor[3] = {
							(float)MenuControls::v_players_esp_enemy_chams->GetCustomColor().r() / 255.f,
							(float)MenuControls::v_players_esp_enemy_chams->GetCustomColor().g() / 255.f,
							(float)MenuControls::v_players_esp_enemy_chams->GetCustomColor().b() / 255.f
						};

						switch (MenuControls::v_players_esp_chams_mode->GetSelectedItem())
						{
						case 0: // Normal
							chams_material = Materials::material_normal;
							break;
						case 1: // Flat
							chams_material = Materials::material_flat;
							break;
						case 2: // Wireframe
							chams_material = Materials::material_wireframe;
							break;
						case 3: // Transparent
							chams_material = Materials::material_normal;
							break;
						default:
							break;
						}

						ent->SetAbsAngles(Vector(btp.BestRecord.angles.x, btp.BestRecord.angles.y, 0));
						ent->SetAbsOrigin(btp.BestRecord.position);

						game_interfaces::RenderView->SetColorModulation(Enemy_ChamsColor);
						game_interfaces::ModelRender->ForcedMaterialOverride(chams_material);

						ent->DrawModel(0x1, instance);

						ent->SetAbsAngles(Vector(backup_angles.x, backup_angles.y, 0));
						ent->SetAbsOrigin(backup_origin);

						game_interfaces::ModelRender->ForcedMaterialOverride(nullptr);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

BackTrackRecord LC::GetBestRecord(BackTrackPlayer &backtrack_player, entity* ent)
{
	bool RecordLBYInited = false;
	float LastLBY = 0.f;

	BackTrackRecord LBYUpdateRecord(-1);
	BackTrackRecord MaxDistanceRecord(-1);

	float MaxDistance = 0.f;

	if (backtrack_player.Records.size() > 0)
	{
		for (BackTrackRecord &record : backtrack_player.Records)
		{
			if (LC::TickIsValid(record.SimulationTime) && record.Hitboxes.size() > 0)
			{
				if (RecordLBYInited)
				{
					if (LastLBY != record.LowerBody)
					{
						LBYUpdateRecord = record;
					}
				}
				else {
					LastLBY = record.LowerBody;
					RecordLBYInited = true;
				}

				if (record.Velocity > 5.f)
				{
					LBYUpdateRecord = record;
				}

				if (record.Hitboxes[0].vec.Length() > 0)
				{
					float distance = record.Hitboxes[0].vec.DistToSqr(ent->GetAbsOrigin());
					if (distance > MaxDistance && distance > 0.f)
					{
						MaxDistance = distance;
						MaxDistanceRecord = record;
					}
				}
			}
		}
	}

	if (MenuControls::a_resolver->GetState())
	{
		if (LBYUpdateRecord.ID > -1) //LBY
		{
			LBYUpdateRecord.angles.y = LBYUpdateRecord.LowerBody;
			backtrack_player.BackTrackMode = 2;
			return LBYUpdateRecord;
		}
		else {
			if (MaxDistanceRecord.ID > -1) //NORMAL
			{
				backtrack_player.BackTrackMode = 0;
				return MaxDistanceRecord;
			}
		}
	}
	else {
		if (MaxDistanceRecord.ID > -1) //NORMAL
		{
			backtrack_player.BackTrackMode = 0;
			return MaxDistanceRecord;
		}
	}
	
	return BackTrackRecord(-1);
}

AimbotBackTrackVector LC::GetVectorToShoot(entity* ent, entity* local)
{
	if (BackTrack_Players.size() > 0)
	{
		for (BackTrackPlayer &btp : BackTrack_Players)
		{
			if (btp.PlayerIndex == ent->GetIndex())
			{
				// Check if the best record can be used
				if (btp.BestRecord.ID >= 0 && btp.BestRecord.Hitboxes.size() > 0)
				{
					if (LC::TickIsValid(btp.BestRecord.SimulationTime))
					{
						AimbotHitbox RecordVectorResult = AimbotHitbox(-1, Vector(0, 0, 0));

						RecordVectorResult = GetHittableBackTrackPosition(btp.BestRecord, local);

						if (RecordVectorResult.vec.Length() > 0 && RecordVectorResult.id > -1)
						{
							return AimbotBackTrackVector(RecordVectorResult, btp.BestRecord, btp.BackTrackMode);
						}
					}
				}
			}
		}
	}
	return AimbotBackTrackVector(AimbotHitbox(-1,Vector(0, 0, 0)), BackTrackRecord(-1), 0); // None of the records can be used
}

AimbotHitbox LC::GetHittableBackTrackPosition(BackTrackRecord Record, entity* local) {
	if (Record.Hitboxes.size() > 0)
	{
		for (AimbotHitbox &o : Record.Hitboxes)
		{
			if (o.vec.Length() > 0 && o.id > -1)
			{
				if (IsVisiblePoint(local, o.vec))
				{
					return o;
				}
			}
		}
	}
	if (Record.AutoWallHitboxes.size() > 0)
	{
		if (MenuControls::a_autowall->GetState())
		{
			for (AimbotHitbox &o : Record.AutoWallHitboxes)
			{
				if (o.vec.Length() > 0 && o.id > -1)
				{
					return o;
				}
			}
		}
	}
	return AimbotHitbox(-1,Vector(0, 0, 0));
}

void LC::SetPlayer(entity* ent, BackTrackRecord record)
{
	playerinfo PlayerInfo;
	if (TargetValid(ent))
	{
		if (game_interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &PlayerInfo))
		{
			ent->SetAbsOrigin(record.position);
			ent->SetAbsAngles(record.angles);
		}
	}
}

void LC::UpdateRecord(BackTrackRecord &record, entity* ent, entity* local)
{
	record.AutoWallHitboxes.clear();
	record.Hitboxes.clear();
	record.head_vector = Vector(0, 0, 0);

	record.position = ent->m_vecOrigin();
	record.angles = ent->m_angEyeAngles();
	record.LowerBody = ent->m_flLowerBodyYawTarget();
	record.Velocity = ent->m_vecVelocity().Length();
	record.SimulationTime = ent->m_flSimulationTime(); //ent->extrapolation_simulationtime;

	Vector head_vec;
	if (ent->GetHitboxPosition(Hitboxes::HITBOX_HEAD, head_vec))
	{
		record.head_vector = head_vec;
	}

	if (Aimbot::GetTargetHitboxes().size() > 0)
	{
		for (int hitbox : Aimbot::GetTargetHitboxes())
		{
			Vector res;
			if (ent->GetHitboxPosition(hitbox, res))
			{
				if (ent->IsVisiblePosition(res, ent))
				{
					record.Hitboxes.push_back(AimbotHitbox(hitbox, res)); // Visible HitBoxes
				}
				else {
					if (MenuControls::a_autowall->GetState())
					{
						float dmg = Autowall::GetDamage(res, local, ent);
						if (dmg >= MenuControls::a_mindamage->GetValue())
						{
							record.AutoWallHitboxes.push_back(AimbotHitbox(hitbox, res));
						}
					}
				}
			}
		}
	}
	else {	
		for (int hitbox = 0; hitbox < ent->GetNumHitBoxes(); hitbox++)
		{
			Vector res;
			if (ent->GetHitboxPosition(hitbox, res))
			{
				if (ent->IsVisiblePosition(res, ent))
				{
					record.Hitboxes.push_back(AimbotHitbox(hitbox, res)); // Visible HitBoxes
				}
				else {
					if (MenuControls::a_autowall->GetState())
					{
						float dmg = Autowall::GetDamage(res, local, ent);
						if (dmg >= MenuControls::a_mindamage->GetValue())
						{
							record.AutoWallHitboxes.push_back(AimbotHitbox(hitbox, res));
						}
					}
				}
			}
		}
	}
}

#include "Resolver.h"

void LC::UpdateBackTrackEntity(entity* ent, entity* local)
{
	bool AlreadyInList = false;

	for (BackTrackPlayer &btp : BackTrack_Players)
	{
		if (btp.PlayerIndex == ent->GetIndex())
		{
			AlreadyInList = true;
			bool RecordFound = false;

			for (BackTrackRecord &record : btp.Records)
			{
				if (record.ID == btp.LatestAddedRecord)
				{
					RecordFound = true;
					UpdateRecord(record, ent, local);
				}
			}

			if (!RecordFound)
			{
				BackTrackRecord NewRecord(btp.LatestAddedRecord);
				UpdateRecord(NewRecord, ent, local);
				btp.Records.push_back(NewRecord);
			}

			if (btp.LatestAddedRecord > MaxTicks)
			{
				btp.BestRecord = GetBestRecord(btp, ent);
				btp.LatestAddedRecord = 0;
			}
			else {
				btp.LatestAddedRecord += 1;
			}

		}
	}

	if (!AlreadyInList)
	{
		BackTrackPlayer new_backtrackplayer(ent->GetIndex());
		BackTrack_Players.push_back(new_backtrackplayer);
	}
}

void LC::UpdateBackTrack(entity* local, entity* ent)
{
	UpdateBackTrackEntity(ent, local);
}