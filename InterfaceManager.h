#pragma once

#include "Client.h"
#include "Engine.h"
#include "RenderView.h"
#include "ModelRender.h"
#include "MaterialSystem.h"
#include "EntityList.h"
#include "VGui.h"
#include "ModelInfo.h"
#include "Trace.h"
#include "Globals.h"
#include "PhysicsProps.h"
#include "Input.h"
#include "GameEventManager.h"
#include "Prediction.h"
#include "MoveHelper.h"
#include "GameMovement.h"
#include "ViewRenderBeams.h"
#include "CVar.h"
#include "DebugOverlay.h"
#include "MemAlloc.h"
#include "EngineSound.h"
#include "PlayerResource.h"
#include "StudioRender.h"

typedef void(__cdecl* tier0_Message)(const char*, ...);

namespace game_interfaces
{
	void init();
	bool is_panorama_ui();
	char* get_client_dll();

	extern IClient* Client;
	extern IClientState* ClientState;
	extern IClientMode* ClientMode;
	extern IEngine* Engine;
	extern IRenderView* RenderView;
	extern IModelRender* ModelRender;
	extern IMaterialSystem* MaterialSystem;
	extern IEntityList* EntityList;
	extern vgui::IPanel* VGui;
	extern vgui::ISurface* Surface;
	extern IModelInfo* ModelInfo;
	extern ITraceEngine* Trace;
	extern IGlobalVarsBase* Globals;
	extern IPhysicsProps* PhysicsProps;
	extern IInput* Input;
	extern ICVar* CVar;
	extern IPrediction* Prediction;
	extern IMoveHelper* MoveHelper;
	extern IGameMovement* GameMovement;
	extern IGameEventManager2* GameEvents;
	extern IViewRenderBeams* RenderBeams;
	extern IDebugOverlay* DebugOverlay;
	extern IMemAlloc* MemAlloc;
	//extern tier0_Message Message;
	extern IEngineSound* EngineSound;
	extern C_CSPlayerResource** PlayerResources;
	extern IStudioRender* StudioRender;
	extern uintptr_t view_matrix;
}