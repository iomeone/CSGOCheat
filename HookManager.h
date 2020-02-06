#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <deque>

#include "ModelRender.h"
#include "MaterialUtils.h"
#include "Entity.h"
#include "GameEventListener.h"
#include "VGui.h"
#include "CUserCmd.h"
#include "CVMTHookManager.h"
#include "Vector.h"
#include "Client.h"
#include "RandomStuff.h"
#include "Trace.h"

typedef HRESULT(__stdcall* PresentFn)(IDirect3DDevice9*, const RECT*, RECT*, HWND, RGNDATA*);
typedef HRESULT(__stdcall* ResetFn)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, DrawModelState_t&, ModelRenderInfo_t&, matrix3x4_t*);
typedef void(__fastcall* SceneEndFn)(void*, void*);
typedef void(__thiscall* PaintTraverseFn)(vgui::IPanel*, vgui::VPANEL, bool, bool);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall* FireEventFn) (void*, IGameEvent*, bool);
typedef bool(__thiscall* WriteUserCmdDeltaToBufferFn)(void*, int, bf_write*, int, int, bool);
typedef float(__stdcall* GetLastTimeStampFn)(void);
typedef int(__thiscall* SendDatagramFn)(void*, void *);
typedef void(__thiscall* TraceRayFn)(void*,const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
typedef void(__thiscall* LockCursorFn)(void*);

struct CIncomingSequence
{
	CIncomingSequence::CIncomingSequence(int instate, int outstate, int seqnr, float time)
	{
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};

namespace HookValues
{
	extern std::deque<CIncomingSequence>sequences;
	extern int lastincomingsequencenumber;
	extern Vector RealAimPunchAngle;
	extern float interpolation_disable_timer;
}

namespace hooks
{
	extern CVMTHookManager* Client_Hook;
	extern CVMTHookManager* ClientMode_Hook;
	extern CVMTHookManager* RenderView_Hook;
	extern CVMTHookManager* Prediction_Hook;
	extern CVMTHookManager* VGui_Hook;
	extern CVMTHookManager* Engine_Hook;
	extern CVMTHookManager* ModelRender_Hook;
	extern CVMTHookManager* NetChannel_Hook;
	extern CVMTHookManager* EngineTrace_Hook;
	extern CVMTHookManager* D3DDevice9_Hook;
	extern CVMTHookManager* Surface_Hook;

	extern PresentFn oPresent;
	extern ResetFn oReset;
	extern SendDatagramFn oSendDatagram;
	extern DrawModelExecuteFn oDrawModelExecute;
	extern SceneEndFn oSceneEnd;
	extern PaintTraverseFn oPaintTraverse;
	extern FrameStageNotifyFn oFrameStageNotify;
	extern FireEventFn oFireEvent;
	extern WriteUserCmdDeltaToBufferFn oWriteUserCmdDeltaToBuffer;
	extern GetLastTimeStampFn oGetLastTimeStamp;
	extern TraceRayFn oTraceRay;
	extern LockCursorFn oLockCursor;

	extern HRESULT __stdcall Present_Hook(LPDIRECT3DDEVICE9 Device, RECT* SourceRect, RECT* DestRect, HWND DestWindowOverride, RGNDATA* DirtyRegion);
	extern HRESULT __stdcall Reset_Hook(IDirect3DDevice9* Device, D3DPRESENT_PARAMETERS* Parameters);

	extern void __fastcall DrawModelExecute_Hook(void* thisptr, void* edx, IMatRenderContext* ctx, DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	extern bool __stdcall CreateMove_Hook(float frametime, CUserCmd* cmd);
	extern void __fastcall SceneEnd_Hook(void* thisptr, void* edx);
	extern void __stdcall PaintTraverse_Hook(vgui::VPANEL Panel, bool forceRepaint, bool allowForce);
	extern void __stdcall RunCommand_Hook(entity* pEntity, CUserCmd* cmd, void* pMoveHelper);
	extern void  __stdcall FrameStageNotify_Hook(ClientFrameStage_t stage);
	extern float __stdcall GetLastTimeStamp_Hook();
	extern int __fastcall SendDatagram_Hook(INetChannel* netchan, void*, bf_write* datagram);
	extern void __fastcall TraceRay_Hook(void* thisptr, void* ecx,const Ray_t& ray, unsigned int mask, ITraceFilter* trace_filter, trace_t* trace);
	extern void __stdcall LockCursor_Hook();

	extern void UpdateIncomingSequences();
}

namespace hook_manager
{
	void init();
}