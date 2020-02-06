#include "HookManager.h"
#include "InterfaceManager.h"
#include "MemoryUtils.h"
#include "RenderUtils.h"
#include "SkinChanger.h"
#include "LoggerUtils.h"
#include "SecurityUtils.h"

CVMTHookManager* hooks::Client_Hook;
CVMTHookManager* hooks::ClientMode_Hook;
CVMTHookManager* hooks::RenderView_Hook;
CVMTHookManager* hooks::Prediction_Hook;
CVMTHookManager* hooks::VGui_Hook;
CVMTHookManager* hooks::Engine_Hook;
CVMTHookManager* hooks::ModelRender_Hook;
CVMTHookManager* hooks::NetChannel_Hook;
CVMTHookManager* hooks::EngineTrace_Hook;
CVMTHookManager* hooks::D3DDevice9_Hook;
CVMTHookManager* hooks::Surface_Hook;

void hook_manager::init()
{
	hooks::ModelRender_Hook = new CVMTHookManager((PDWORD*)game_interfaces::ModelRender);
	hooks::oDrawModelExecute = (DrawModelExecuteFn)hooks::ModelRender_Hook->HookMethod((DWORD)&hooks::DrawModelExecute_Hook, 21); // Used to change the model transparency 
	Logger::SaveToLog("Hooked drawmodelexecute");
	hooks::Engine_Hook = new CVMTHookManager((PDWORD*)game_interfaces::Engine);
	hooks::oGetLastTimeStamp = (GetLastTimeStampFn)hooks::Engine_Hook->HookMethod((DWORD)hooks::GetLastTimeStamp_Hook, 14); // Used to disable interpolation
	Logger::SaveToLog("Hooked engine");
	hooks::Prediction_Hook = new CVMTHookManager((PDWORD*)game_interfaces::Prediction);
	hooks::Prediction_Hook->HookMethod((DWORD)&hooks::RunCommand_Hook, 19); // Used to get the movehelper (to avoid using signatures)
	Logger::SaveToLog("Hooked prediction");
	hooks::Client_Hook = new CVMTHookManager((PDWORD*)game_interfaces::Client);
	hooks::oFrameStageNotify = (FrameStageNotifyFn)hooks::Client_Hook->HookMethod((DWORD)&hooks::FrameStageNotify_Hook, 37); // Used for changing player stuff (Resolver, NoSmoke, PVS Fix)
	Logger::SaveToLog("Hooked framestagenotify");
	hooks::ClientMode_Hook = new CVMTHookManager((PDWORD*)game_interfaces::ClientMode);
	hooks::ClientMode_Hook->HookMethod((DWORD)&hooks::CreateMove_Hook, 24); // Used for movement using cmd related features
	Logger::SaveToLog("Hooked clientmode");
	hooks::RenderView_Hook = new CVMTHookManager((PDWORD*)game_interfaces::RenderView);
	hooks::oSceneEnd = (SceneEndFn)hooks::RenderView_Hook->HookMethod((DWORD)&hooks::SceneEnd_Hook, 9); // Used for chams
	Logger::SaveToLog("Hooked sceneend");
	hooks::VGui_Hook = new CVMTHookManager((PDWORD*)game_interfaces::VGui);
	hooks::oPaintTraverse = (PaintTraverseFn)hooks::VGui_Hook->HookMethod((DWORD)&hooks::PaintTraverse_Hook, 41); // Used to cancel some panels
	Logger::SaveToLog("Hooked painttraverse");
	hooks::Surface_Hook = new CVMTHookManager((PDWORD*)game_interfaces::Surface);
	hooks::oLockCursor = (LockCursorFn)hooks::Surface_Hook->HookMethod((DWORD)&hooks::LockCursor_Hook, 67); // Used to enable or disable the cursor 
	Logger::SaveToLog("Hooked surface");
	hooks::EngineTrace_Hook = new CVMTHookManager((PDWORD*)game_interfaces::Trace);
	hooks::oTraceRay = (TraceRayFn)hooks::EngineTrace_Hook->HookMethod((DWORD)&hooks::TraceRay_Hook, 5); // Used for fixing an autowall bug
	Logger::SaveToLog("Hooked traceray");
	static DWORD_PTR* present_addr = (DWORD_PTR*)(MemoryUtils::FindBytePattern("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB") + 2);
	static DWORD_PTR* reset_addr = (DWORD_PTR*)(MemoryUtils::FindBytePattern("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 2);
	hooks::oPresent = **reinterpret_cast<decltype(&hooks::oPresent)*>(present_addr);
	**reinterpret_cast<void***>(present_addr) = reinterpret_cast<void*>(&hooks::Present_Hook);

	hooks::oReset = **reinterpret_cast<decltype(&hooks::oReset)*>(reset_addr);
	**reinterpret_cast<void***>(reset_addr) = reinterpret_cast<void*>(&hooks::Reset_Hook);
	Logger::SaveToLog("Hooked directx");
	////////////////////////////////////////////////////////////////////////////////////////////////////

	RecvProp* SequenceProp = netvars->GetNetvarProp("DT_BaseViewModel", "m_nSequence");
	SequenceProp->SetProxyFn(SkinChanger::RecvProxy_nSequence);
	RecvProp* ModelIndexProp = netvars->GetNetvarProp("DT_BaseViewModel", "m_nModelIndex");
	ModelIndexProp->SetProxyFn(SkinChanger::RecvProxy_Viewmodel);
	Logger::SaveToLog("Hooked skinchanger netvars");
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//SpoofedConvar* spoofed_developer = new SpoofedConvar(game_interfaces::CVar->FindVar("developer"));
	//if (spoofed_developer)
	//{
	//	spoofed_developer->SetString("1");
	//}
	//SpoofedConvar* spoofed_filter = new SpoofedConvar(game_interfaces::CVar->FindVar("con_filter_enable"));
	//if (spoofed_filter)
	//{
	//	spoofed_filter->SetString("1");
	//}
	//SpoofedConvar* spoofed_filter_text = new SpoofedConvar(game_interfaces::CVar->FindVar("con_filter_text"));
	//if (spoofed_filter_text) 
	//{
	//	spoofed_filter_text->SetString("[dominance]");
	//}	


	////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string welcome_string = "[dominance] Welcome " + SecurityUtils::username + " \n"; 
	Logger::Message(welcome_string, Color::White());
}