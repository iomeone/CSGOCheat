#pragma once

#include "ClientClass.h"
#include "RandomStuff.h"
#include "Vector.h"
#include "MemoryUtils.h"

class IClient
{
public:
	ClientClass* GetClasses()
	{
		typedef ClientClass* (__thiscall* original)(PVOID);
		return vfunction<original>(this, 8)(this);
	}
};

class IClientMode
{
public:
};

class INetChannel
{
public:
	bool Transmit(bool onlyReliable)
	{
		typedef bool(__thiscall* original)(PVOID, bool);
		return vfunction< original >(this, 49)(this, onlyReliable);
	}
	__int32 vtable; //0x0000
	__int32* msgbinder1; //0x0004
	__int32* msgbinder2; //0x0008
	__int32* msgbinder3; //0x000C
	__int32* msgbinder4; //0x0010
	unsigned char m_bProcessingMessages; //0x0014
	unsigned char m_bShouldDelete; //0x0015
	char pad_0x0016[0x2]; //0x0016
	__int32 m_nOutSequenceNr; //0x0018
	__int32 m_nInSequenceNr; //0x001C
	__int32 m_nOutSequenceNrAck; //0x0020
	__int32 m_nOutReliableState; //0x0024
	__int32 m_nInReliableState; //0x0028
	__int32 m_nChokedPackets; //0x002C
	char pad_0030[1044];         //0x0030
}; //Size: 0x0444

class CClockDriftMgr
{
public:
	float m_ClockOffsets[17];   //0x0000
	uint32_t m_iCurClockOffset; //0x0044
	uint32_t m_nServerTick;     //0x0048
	uint32_t m_nClientTick;     //0x004C
}; //Size: 0x0050

class IClientState
{
public:
	void ForceFullUpdate()
	{
		*(uint32_t*)((uintptr_t)this + 0x174) = -1;
	}
	uint32_t m_nDeltaTick()
	{
		return *(uint32_t*)((uintptr_t)this + 0x174);
	}
	uint32_t lastoutgoingcommand()
	{
		return *(uint32_t*)((uintptr_t)this + 0x4D24);
	}
	uint32_t chokedcommands()
	{
		return *(uint32_t*)((uintptr_t)this + 0x4D28);
	}
	uint32_t last_command_ack()
	{
		return *(uint32_t*)((uintptr_t)this + 0x4D2C);
	}
	uint32_t command_ack()
	{
		return *(uint32_t*)((uintptr_t)this + 0x4D30);
	}
	INetChannel* m_NetChannel()
	{
		return (INetChannel*)((uintptr_t)this + 0x9C);
	}
}; 
