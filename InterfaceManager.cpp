#include "InterfaceManager.h"
#include "XorString\XorStr.hpp"
#include "EventListener.h"
#include "SecurityUtils.h"
#include "MemoryUtils.h"
#include <Windows.h>

typedef void* (*Instantiateinterface)();
class CInterface
{
public:
	Instantiateinterface Interface;
	char* Interfacename;
	CInterface* Nextinterface;
};

void* get_interface(const char* _Module, const char* _Object)
{
	ULONG createinterface = (ULONG)GetProcAddress(GetModuleHandle(_Module), /*CreateInterface*/XorStr(15, 0x4A, 0x09BB2DA6, 0x32A00DAD, 0x36A432D9, 0x5FDE5900).c_str());
	ULONG shortjump = (ULONG)createinterface + 5;
	ULONG jump = (((ULONG)createinterface + 5) + *(ULONG*)shortjump) + 4;
	CInterface* list = **(CInterface***)(jump + 6);

	while (list)
	{
		if (strstr(list->Interfacename, _Object) && (strlen(list->Interfacename) - strlen(_Object)) < 5)
		{
			return list->Interface();
			break;
		}

		list = list->Nextinterface;
	}

	return 0;
}

std::string get_interface_string_(std::string interface_)
{
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	std::string web_domain = /*dominanced.com*/XorStr(14, 0x7E, 0x1A921192, 0x14981694, 0x13915A90, 0x1D9C0000);
	std::string php_request = /*launcher/get_cheat_data_php.php?sess=*/XorStr(37, 0xD5, 0xB935A63C, 0xB238AA3C, 0xE22BAE3E, 0x962BAF23, 0xA4309C26, 0xA034DE61, 0xCD54CB14, 0xC950C709, 0xC651C041, 0x8C000000);
	std::string php_argument = /*&interface=*/XorStr(11, 0x64, 0x428A0C95, 0x05AD38BC, 0x3FBE6700);
	std::string start_php = php_request + SecurityUtils::get_session_id() + php_argument + interface_;
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	std::cout << start_php.c_str() << std::endl;
	return SecurityUtils::get_web_response(web_domain, start_php);
}

bool game_interfaces::is_panorama_ui()
{
	return GetModuleHandleA("client_panorama.dll");
}

char* game_interfaces::get_client_dll()
{
	return is_panorama_ui() ? "client_panorama.dll" : "client.dll";
}

void game_interfaces::init()
{
	Logger::SaveToLog("InterfaceManager started");
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	game_interfaces::Client = reinterpret_cast<IClient*>(get_interface(get_client_dll(), "VClient"));
	Logger::SaveToLog("Trying to get client pointer -> VClient");
	game_interfaces::ClientState = reinterpret_cast<IClientState*>(MemoryUtils::FindBytePattern("engine.dll", "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0"));
	Logger::SaveToLog("Trying to get clientstate pointer -> A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0");
	game_interfaces::ClientMode = **reinterpret_cast<IClientMode***>(MemoryUtils::FindBytePattern(get_client_dll(),"8B 35 ? ? ? ? 8B 80") + 2);
	game_interfaces::Engine = reinterpret_cast<IEngine*>(get_interface("engine.dll", "VEngineClient"));
	Logger::SaveToLog("Trying to get engine pointer -> VEngineClient");
	game_interfaces::RenderView = reinterpret_cast<IRenderView*>(get_interface("engine.dll", "VEngineRenderView"));
	Logger::SaveToLog("Trying to get renderview pointer -> VEngineRenderView");
	game_interfaces::ModelRender = reinterpret_cast<IModelRender*>(get_interface("engine.dll", "VEngineModel"));
	Logger::SaveToLog("Trying to get enginemodel pointer -> VEngineModel");
	game_interfaces::MaterialSystem = reinterpret_cast<IMaterialSystem*>(get_interface("materialsystem.dll", "VMaterialSystem"));
	Logger::SaveToLog("Trying to get the other pointers...");
	game_interfaces::EntityList = reinterpret_cast<IEntityList*>(get_interface(get_client_dll(), "VClientEntityList"));
	game_interfaces::VGui = reinterpret_cast<vgui::IPanel*>(get_interface("vgui2.dll", "VGUI_Panel"));
	game_interfaces::Surface = reinterpret_cast<vgui::ISurface*>(get_interface("vguimatsurface.dll", "VGUI_Surface"));
	game_interfaces::ModelInfo = reinterpret_cast<IModelInfo*>(get_interface("engine.dll", "VModelInfoClient"));
	game_interfaces::Trace = reinterpret_cast<ITraceEngine*>(get_interface("engine.dll", "EngineTraceClient"));
	game_interfaces::PhysicsProps = reinterpret_cast<IPhysicsProps*>(get_interface("vphysics.dll", "VPhysicsSurfaceProps"));
	game_interfaces::Globals = **reinterpret_cast<IGlobalVarsBase***>((*(DWORD**)game_interfaces::Client)[0] + 0x1B);
	game_interfaces::Prediction = reinterpret_cast<IPrediction*>(get_interface(get_client_dll(), "VClientPrediction"));
	game_interfaces::GameMovement = reinterpret_cast<IGameMovement*>(get_interface(get_client_dll(), "GameMovement"));
	game_interfaces::Input = *reinterpret_cast<IInput**>(MemoryUtils::FindBytePattern(get_client_dll(),"B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);
	game_interfaces::CVar = reinterpret_cast<ICVar*>(get_interface("vstdlib.dll", "VEngineCvar"));
	game_interfaces::GameEvents = reinterpret_cast<IGameEventManager2*>(get_interface("engine.dll", "GAMEEVENTSMANAGER002"));
	game_interfaces::DebugOverlay = reinterpret_cast<IDebugOverlay*>(get_interface("engine.dll", "VDebugOverlay"));
	game_interfaces::MemAlloc = *reinterpret_cast<IMemAlloc**>(GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc"));
	//game_interfaces::Message = reinterpret_cast<tier0_Message>(GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"));
	game_interfaces::EngineSound = reinterpret_cast<IEngineSound*>(get_interface("engine.dll", "IEngineSoundClient"));
	game_interfaces::StudioRender = reinterpret_cast<IStudioRender*>(get_interface("studiorender.dll", "VStudioRender"));
	game_interfaces::PlayerResources = (C_CSPlayerResource**)(0x2EC5E2C + reinterpret_cast<DWORD>(GetModuleHandleA(get_client_dll())));
	game_interfaces::RenderBeams = *reinterpret_cast<IViewRenderBeams**>((DWORD)MemoryUtils::FindBytePattern(get_client_dll(), "8D 43 FC B9 ? ? ? ? 50 A1") + 4);
	game_interfaces::view_matrix = MemoryUtils::FindBytePattern(get_client_dll(), "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9");
	game_interfaces::view_matrix = *reinterpret_cast< uintptr_t* >(view_matrix + 0x3) + 176;
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	HitListener* listener = new HitListener;
	BulletImpactListener* listener2 = new BulletImpactListener;
	BuyListener* listener3 = new BuyListener;
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	Logger::SaveToLog("All interfaces created");
}

namespace game_interfaces
{
	IClient* Client;
	IClientState* ClientState;
	IClientMode* ClientMode;
	IEngine* Engine;
	IRenderView* RenderView;
	IModelRender* ModelRender;
	IMaterialSystem* MaterialSystem;
	IEntityList* EntityList;
	vgui::IPanel* VGui;
	vgui::ISurface* Surface;
	IModelInfo* ModelInfo;
	ITraceEngine* Trace;
	IGlobalVarsBase* Globals;
	IPhysicsProps* PhysicsProps;
	IInput* Input;
	ICVar* CVar;
	IPrediction* Prediction;
	IMoveHelper* MoveHelper;
	IGameMovement* GameMovement;
	IGameEventManager2* GameEvents;
	IViewRenderBeams* RenderBeams;
	IDebugOverlay* DebugOverlay;
	IMemAlloc* MemAlloc;
	//tier0_Message Message;
	IEngineSound* EngineSound;
	C_CSPlayerResource** PlayerResources;
	IStudioRender* StudioRender;
	uintptr_t view_matrix;
}