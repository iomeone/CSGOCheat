#include "EPrediction.h"

#include "InterfaceManager.h"
#include "RandomStuff.h"

#include <iostream>
#include "MemoryUtils.h"

#include "checksum_md5.h"

int* SetPredictionPlayer;
int* PredictionRandomSeed;

float OriginalFrameTime;
float OriginalCurTime;
static char* cMoveData = nullptr;

int iTickBaseBackup;
int iFlagsBackup;
int iButtonsBackup;

CUserCmd* CurrentCommand(entity* ent)
{
	static auto currentCommand = *(uint32_t*)(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *(CUserCmd**)((uintptr_t)ent + currentCommand);
}

void patternstuff(entity* local, CUserCmd* cmd)
{
	
}

void Prediction::start(CUserCmd* cmd, entity* local)
{
	if (!game_interfaces::MoveHelper)
	{
		std::cout << "Searching MoveHelper" << std::endl;
		return;
	}

	iFlagsBackup = local->m_fFlags();
	//iButtonsBackup = cmd->buttons;

	if (!SetPredictionPlayer)
		SetPredictionPlayer = *reinterpret_cast<int**>(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), game_interfaces::is_panorama_ui() ? "89 35 ? ? ? ? F3 0F 10 48 20" : "89 3D ? ? ? ? F3 0F 10 47") + 2);

	if (!PredictionRandomSeed)
		PredictionRandomSeed = *reinterpret_cast<int**>(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);

	*PredictionRandomSeed = MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;
	*SetPredictionPlayer = uintptr_t(local);

	// Backup
	OriginalFrameTime = game_interfaces::Globals->frametime;
	OriginalCurTime = game_interfaces::Globals->curtime;

	if (!cMoveData)
		cMoveData = (char*)(calloc(1, sizeof(CMoveData)));

	// We make our new predicted values and set them
	game_interfaces::Globals->curtime = local->GetCorrectedTickBase() * game_interfaces::Globals->interval_per_tick;
	game_interfaces::Globals->frametime = game_interfaces::Globals->interval_per_tick;

	// We start to predict the track prediction errors
	game_interfaces::GameMovement->StartTrackPredictionErrors(local);

	// We set the host
	game_interfaces::MoveHelper->SetHost(local);

	iTickBaseBackup = local->m_nTickBase();

	// We make our movement prediction
	game_interfaces::Prediction->SetupMove(local, cmd, game_interfaces::MoveHelper, (CMoveData*)cMoveData);
	game_interfaces::GameMovement->ProcessMovement(local, (CMoveData*)cMoveData);
	game_interfaces::Prediction->FinishMove(local, cmd, (CMoveData*)cMoveData);
}

void Prediction::stop(entity* local, CUserCmd* cmd)
{
	if (!game_interfaces::MoveHelper)
		return;

	// We finish the track prediction errors
	game_interfaces::GameMovement->FinishTrackPredictionErrors(local);

	// We reset the host
	game_interfaces::MoveHelper->SetHost(NULL);

	// We reset the pattern stuff
	if (PredictionRandomSeed)
	{
		*PredictionRandomSeed = -1;
	}
	if (SetPredictionPlayer)
	{
		*SetPredictionPlayer = 0;
	}

	// We set our backup values
	game_interfaces::Globals->frametime = OriginalFrameTime;
	game_interfaces::Globals->curtime = OriginalCurTime;

	//Last but not least, set these to their backups 
	local->m_fFlags() = iFlagsBackup;
	local->m_nTickBase() = iTickBaseBackup;
	//cmd->buttons = iButtonsBackup;

}