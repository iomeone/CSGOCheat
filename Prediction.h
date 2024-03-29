#pragma once

#include "Entity.h"
#include "CUserCmd.h"
#include "MoveHelper.h"
#include "RandomStuff.h"

class IPrediction
{
public:
	// Construction
public:

	virtual ~IPrediction(void) = 0;//

	virtual void Init(void) = 0;//
	virtual void Shutdown(void) = 0;//

									// Implement IPrediction
public:

	virtual void Update
	(
		int startframe, // World update ( un-modded ) most recently received
		bool validframe, // Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command // Last command (most recent) sent to server (un-modded)
	);//

	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet);//
	virtual void PostEntityPacketReceived(void);//5
	virtual void PostNetworkDataReceived(int commands_acknowledged);//

	virtual void OnReceivedUncompressedPacket(void);//

													// The engine needs to be able to access a few predicted values
	virtual void GetViewOrigin(Vector& org);//
	virtual void SetViewOrigin(Vector& org);//
	virtual void GetViewAngles(Vector& ang);//10
	virtual void SetViewAngles(Vector& ang);//

	virtual void GetLocalViewAngles(Vector& ang);//
	virtual void SetLocalViewAngles(Vector& ang);//

	virtual bool InPrediction(void) const;//14
	virtual bool IsFirstTimePredicted(void) const;//

	virtual int GetLastAcknowledgedCommandNumber(void) const;//

	virtual int GetIncomingPacketNumber(void) const;//

	virtual void CheckMovingGround(entity* player, double frametime);//
	virtual void RunCommand(entity* player, CUserCmd* cmd, IMoveHelper* moveHelper);//

	virtual void SetupMove(entity* player, CUserCmd* cmd, IMoveHelper* pHelper, CMoveData* move);//20
	virtual void FinishMove(entity* player, CUserCmd* cmd, CMoveData* move);//
	virtual void SetIdealPitch(int nSlot, entity* player, const Vector& origin, const Vector& angles, const Vector& viewheight);//

	virtual void CheckError(int nSlot, entity* player, int commands_acknowledged);//

	virtual void _Update
	(
		int nSlot,
		bool received_new_world_update,
		bool validframe,            // Is frame data valid
		int incoming_acknowledged,  // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command        // Last command (most recent) sent to server (un-modded)
	);
};