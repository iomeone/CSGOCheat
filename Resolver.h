#pragma once

#include "Entity.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ResolverBOOLEAN
{
	ResolverBOOLEAN(bool b)
	{
		this->boolean = b;
		this->angle = -999;
	}
	ResolverBOOLEAN(bool b, float a)
	{
		this->boolean = b;
		this->angle = a;
	}
	bool boolean;
	float angle;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum Spread_AnimationResult
{
	Spread_RELOADING,
	Spread_BREAKING_LBY_OVER_120,
	Spread_BREAKING_LBY_UNDER_120,
	Spread_STOPPED_MOVING,
	Spread_BREAKING_IN,
	Spread_NOTHING
};

enum Spread_ResolverState
{
	Spread_LBY,
	Spread_BREAK,
	Spread_STATIC,
	Spread_PREDICTED,
	Spread_LASTMOVING,
	Spread_NONE,
};

enum Spin_Direction
{
	SPIN_POSITIVE,
	SPIN_NEGATIVE,
	SPIN_NONE
};

struct SpreadConversion
{
	SpreadConversion(Spread_ResolverState mode, float angle)
	{
		this->angle = angle;
		this->mode = mode;
	}
	Spread_ResolverState mode;
	float angle;
};

struct SpreadInfo
{
	float R_LBYTimer_old_curtime;
	bool R_LBYTimer_lby_updated;
	bool R_LBYTimer_was_moving;

	float LastLBY;
	float LastLBYDelta;
	float LastLBYCurtime;
	float LastMovingLBY;

	bool BreakingLBY;
	float BalanceTimer;

	std::vector<float> LBYLog;
	float LBYLogTimer;
	bool LBYStatic;

	float R_LastSpinUpdate;
	float R_SpinTimer;
	float R_LastSpinTimerResult;
	float R_SpinAnglesPerUpdate;
	Spin_Direction R_SpinDirection;
	bool R_CanBeSpinning;

	bool LastMovingLBYCanBeUsed;

	float ResolverAngle;
	int ResolveMode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum NoSpread_ResolverState
{
	NoSpread_LBY,
	NoSpread_BRUTEFORCE_HEIGHT,
	NoSpread_BRUTEFORCE_NORMAL,
	NoSpread_BRUTEFORCE_LASTHIT,

	NoSpread_BRUTEFORCE_SPIN,
	NoSpread_BRUTEFORCE_EXPERIMENTAL,
	NoSpread_BRUTEFORCE_STEP,

	NoSpread_BRUTEFORCE_STATIC,
	NoSpread_BRUTEFORCE_TOTAL
};

struct NoSpreadInfo_Test
{
	float R_LastGroundLby;

	float ResolverAngle;
	int ResolveMode;
};

struct NoSpreadInfo_Step
{
	float R_LastStep;
	float R_Bruteforce_Timer;

	float ResolverAngle;
	int ResolveMode;
};

struct NoSpreadInfo_Brute
{	
	//////////////////////
	float R_LastLowerBody;
	int R_Bruteforce_Mode;
	float R_Bruteforce_Timer;
	float R_LastHittedAngle;
	float R_LastHitTimer;
	//////////////////////

	float ResolverAngle;
	int ResolveMode;
};

struct NoSPreadInfo_Experimental
{
	std::vector<float> R_LowerbodyLog;
	float R_LastLowerBody;
	int R_Bruteforce_step;
	float R_Bruteforce_timer;

	float ResolverAngle;
	int ResolveMode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ResolverPlayer
{
	ResolverPlayer(int index)
	{
		Spread.ResolveMode = 0;
		Spread.ResolverAngle = 0;
		NoSpread_Brute.ResolveMode = 0;
		NoSpread_Brute.ResolverAngle = 0;
		NoSpread_Step.ResolveMode = 0;
		NoSpread_Step.ResolverAngle = 0;
		NoSpread_Step.R_LastStep = 0;
		NoSpread_LBY.ResolveMode = 0;
		NoSpread_LBY.ResolverAngle = 0;
		NoSpread_Experimental.ResolveMode = 0;
		NoSpread_Experimental.ResolverAngle = 0;
		PlayerIndex = index;
	}

	////////////////////////////////

	SpreadInfo Spread;
	NoSpreadInfo_Brute NoSpread_Brute;
	NoSpreadInfo_Step NoSpread_Step;
	NoSpreadInfo_Test NoSpread_LBY;
	NoSPreadInfo_Experimental NoSpread_Experimental;

	////////////////////////////////

	Vector R_VisibleAngle;
	int R_PitchMode;
	int R_PitchTimer;

	////////////////////////////////

	int PlayerIndex;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace SpreadResolver
{
	SpreadConversion ConversionToResolverState(entity* ent, entity* local,ResolverPlayer &res);
	
	void UpdateBreakingLBYState(entity* ent, ResolverPlayer &res);
	void UpdateStaticLBYState(entity* ent, ResolverPlayer &res);
	ResolverBOOLEAN UpdateLBYPredictionState(entity* ent, ResolverPlayer &res);
	void UpdateStoppedLBYState(entity* ent, ResolverPlayer &res);

	bool IsAntiAimOn(entity* ent, ResolverPlayer &res);
	void Main_Update(entity* ent, ResolverPlayer &res, entity* local);
	void LBY_Update(entity* ent, ResolverPlayer &res);
	
	void Draw(entity* ent);
}

namespace NoSpreadResolver_View
{
	void Main_Update(entity* ent, ResolverPlayer &res, entity* local);
	void Draw(entity* ent);
}

namespace NoSpreadResolver_Step
{
	void Main_Update(entity* ent, ResolverPlayer &res, entity* local);
	void Draw(entity* ent);
}

namespace NoSpreadResolver_LBY
{
	void Main_Update(entity* ent, ResolverPlayer &res, entity* local);
	void Draw(entity* ent);
}

namespace NoSpreadResolver_Experimental
{
	void Main_Update(entity* ent, ResolverPlayer &res, entity* local);
	void Draw(entity* ent);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Resolver
{
	extern std::vector<ResolverPlayer> Resolver_Players;

	bool IsUnresolved(entity* ent);
	bool TargetValid(entity* ent);
	void Set(entity* local, entity* ent);
	void Draw(entity* ent);
	bool TargetValid(entity* ent);
	void Resolve(entity* ent);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

