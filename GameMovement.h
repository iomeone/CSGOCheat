#pragma once

#include "Entity.h"
#include "RandomStuff.h"

class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	virtual void	          ProcessMovement(entity* Player, CMoveData *Move) = 0;
	virtual void	          Reset(void) = 0;
	virtual void	          StartTrackPredictionErrors(entity* Player) = 0;
	virtual void	          FinishTrackPredictionErrors(entity* Player) = 0;
	virtual void	          DiffPrint(char const *fmt, ...) = 0;
	virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
	virtual bool		        IsMovingPlayerStuck(void) const = 0;
	virtual entity*   GetMovingPlayer(void) const = 0;
	virtual void		        UnblockPusher(entity* Player, entity* Pusher) = 0;
	virtual void            SetupMovementBounds(CMoveData* Move) = 0;
};

