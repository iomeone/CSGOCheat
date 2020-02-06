#include "Resolver.h"
#include "LagCompensation.h"

#include "InterfaceManager.h"
#include "RenderUtils.h"

#include "Menu.h"

#include "Visuals.h"

#include "Aimbot.h"

void SpreadResolver::Draw(entity* ent)
{
	if (Resolver::TargetValid(ent))
	{
		for (ResolverPlayer &res : Resolver::Resolver_Players)
		{
			if (res.PlayerIndex == ent->GetIndex())
			{
				std::string ResolveMode = "";

				switch (res.Spread.ResolveMode)
				{
				case Spread_LBY:
					ResolveMode += "LBY";
					break;
				case Spread_BREAK:
					ResolveMode += "LBY BROKEN";
					break;
				case Spread_STATIC:
					ResolveMode += "STATIC";
					break;
				case Spread_PREDICTED:
					ResolveMode += "PREDICTION";
					break;
				case Spread_LASTMOVING:
					ResolveMode += "LBY MOVING";
					break;
				case Spread_NONE:
					ResolveMode += "NONE";
					break;
				default:
					ResolveMode = "";
					break;
				}
				Visuals::ESPInfo_List.push_back(ESPInfo(ent->GetIndex(), ResolveMode));
			}
		}
	}
}

bool SpreadResolver::IsAntiAimOn(entity* ent, ResolverPlayer &res)
{
	if (Resolver::TargetValid(ent))
	{
		float diff = res.R_VisibleAngle.y - ent->m_flLowerBodyYawTarget();
		if (fabs(diff) <= 35.f)
		{
			return false;
		}
	}
	return true;
}

void SpreadResolver::LBY_Update(entity* ent, ResolverPlayer &res)
{
	if (Resolver::TargetValid(ent))
	{
		bool moving = ent->m_vecVelocity().Length2D() > 0.1f; // Local is moving
		bool on_ground = ent->m_fFlags() & FL_ONGROUND;
		float predict_time = res.Spread.R_LBYTimer_was_moving ? 0.22f : 1.1f; // Next update

		if (!on_ground)
		{
			res.Spread.R_LBYTimer_lby_updated = false; // LBY isn't updated
			res.Spread.R_LBYTimer_old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
		}
		else {
			if (moving) // Local is moving
			{
				res.Spread.R_LBYTimer_was_moving = true; // Was moving
				res.Spread.R_LBYTimer_lby_updated = true; // LBY updated

				res.Spread.R_LBYTimer_old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
			}
			else {
				if (game_interfaces::Globals->curtime - res.Spread.R_LBYTimer_old_curtime > predict_time) // LBY is updating
				{
					res.Spread.R_LBYTimer_lby_updated = true; // LBY updated
					res.Spread.R_LBYTimer_was_moving = false; // Not moving anymore

					res.Spread.R_LBYTimer_old_curtime = game_interfaces::Globals->curtime; // We update the curtime for the next update
				}
				else {
					res.Spread.R_LBYTimer_lby_updated = false; // LBY not updated
				}
			}
		}
	}
}

void SpreadResolver::UpdateBreakingLBYState(entity* ent, ResolverPlayer &res)
{
	bool animation_found = false;
	for (int i = 0; i < ent->GetNumAnimOverlays(); i++)
	{
		animationlayer& layer = ent->GetAnimOverlay(i);
		int activity = ent->GetSequenceActivity(layer.m_nSequence);

		if (activity >= 0)
		{
			if (activity == 979) // TURN_BALANCEADJUST (real delta to lby > 120)
			{
				animation_found = true;
				if (layer.m_flCycle > 0.25f && layer.m_flCycle < 0.9f)
				{
					res.Spread.BalanceTimer += 1.f;
				}
				else {
					res.Spread.BalanceTimer = 0.f;
				}
			}
		}
	}
	if (!animation_found)
	{
		res.Spread.BalanceTimer = 0.f; 
	}

	if (res.Spread.BalanceTimer > 5.f && ent->m_vecVelocity().Length2D() < 1.f) // Breaking
	{
		res.Spread.BreakingLBY = true;
	}
	else { // Not breaking
		res.Spread.BreakingLBY = false;
	}

}
#include "LoggerUtils.h"
ResolverBOOLEAN SpreadResolver::UpdateLBYPredictionState(entity* ent, ResolverPlayer &res)
{
	if (ent->m_flLowerBodyYawTarget() != res.Spread.R_LastSpinUpdate)
	{
		float diff = ent->m_flLowerBodyYawTarget() - res.Spread.R_LastSpinUpdate;

		res.Spread.R_LastSpinUpdate = ent->m_flLowerBodyYawTarget();
	
		res.Spread.R_LastSpinTimerResult = res.Spread.R_SpinTimer;
		res.Spread.R_SpinTimer = 0.f;

		res.Spread.R_SpinAnglesPerUpdate = fabs(diff);
		res.Spread.R_CanBeSpinning = true;

		res.Spread.R_SpinDirection = diff > 0.f ? Spin_Direction::SPIN_POSITIVE : Spin_Direction::SPIN_NEGATIVE;
	}
	else {
		res.Spread.R_SpinTimer += 1.f;
	}

	if (fabs(res.Spread.R_SpinAnglesPerUpdate) > 35.f && res.Spread.R_SpinTimer <= res.Spread.R_LastSpinTimerResult && res.Spread.R_CanBeSpinning)
	{
		float predicted_step = (res.Spread.R_SpinTimer / res.Spread.R_LastSpinTimerResult) * res.Spread.R_SpinAnglesPerUpdate;
		float predicted_angle = res.Spread.R_SpinDirection == Spin_Direction::SPIN_POSITIVE ? ent->m_flLowerBodyYawTarget() + predicted_step : ent->m_flLowerBodyYawTarget() - predicted_step;
		return ResolverBOOLEAN(true, predicted_angle);
	}
	return ResolverBOOLEAN(false);
}

void SpreadResolver::UpdateStaticLBYState(entity* ent, ResolverPlayer &res)
{
	if (res.Spread.LBYLogTimer < 50.f)
	{
		res.Spread.LBYLogTimer += 1.f;
	}
	else {
		res.Spread.LBYLog.push_back(ent->m_flLowerBodyYawTarget());
		res.Spread.LBYLogTimer = 0;
	}

	if (res.Spread.LBYLog.size() > 10)
	{
		bool did_change = false;
		float last_a = res.Spread.LBYLog[0];
		for (float a : res.Spread.LBYLog)
		{
			if (last_a != a)
			{
				did_change = true;
			}
		}

		res.Spread.LBYStatic = !did_change;

		res.Spread.LBYLog.clear();
	}
}

void SpreadResolver::UpdateStoppedLBYState(entity* ent, ResolverPlayer &res)
{
	bool stopped_moving = false;
	for (int i = 0; i < ent->GetNumAnimOverlays(); i++)
	{
		animationlayer& layer = ent->GetAnimOverlay(i);
		int activity = ent->GetSequenceActivity(layer.m_nSequence);
	
		if (activity >= 0)
		{
			if (activity == 980) // ADJUST_STOPPEDMOVING (stopped moving)
			{
				if (layer.m_flCycle < 0.5 && layer.m_flCycle > 0.f)
				{
					stopped_moving = true;
				}
			}
		}
	}
	res.Spread.LastMovingLBYCanBeUsed = stopped_moving;
}

SpreadConversion SpreadResolver::ConversionToResolverState(entity* ent, entity* local, ResolverPlayer &res)
{
	SpreadResolver::UpdateBreakingLBYState(ent, res);
	SpreadResolver::UpdateStaticLBYState(ent, res);
	ResolverBOOLEAN prediction_response = SpreadResolver::UpdateLBYPredictionState(ent, res);
	SpreadResolver::UpdateStoppedLBYState(ent, res);

	bool on_ground = ent->m_fFlags() & FL_ONGROUND;
	bool moving = ent->m_vecVelocity().Length2D() > 1.f;

	if (on_ground) // Not moving and on the ground
	{
		if (res.Spread.BreakingLBY)
		{
			return SpreadConversion(Spread_ResolverState::Spread_BREAK, ent->m_flLowerBodyYawTarget() - 120.f);
		}
		else {
			if (moving || ent->m_flLowerBodyYawTarget() != res.Spread.LastLBY)
			{
				float Delta = ent->m_flLowerBodyYawTarget() - res.Spread.LastLBY;;
				res.Spread.LastLBYDelta = Delta;
				res.Spread.LastLBYCurtime = game_interfaces::Globals->curtime;
				res.Spread.LastLBY = ent->m_flLowerBodyYawTarget();
				return SpreadConversion(Spread_ResolverState::Spread_LBY, ent->m_flLowerBodyYawTarget());
			}
			else {
				if (prediction_response.boolean)
				{
					return SpreadConversion(Spread_ResolverState::Spread_PREDICTED, prediction_response.angle);
				}
				else {
					if (res.Spread.LBYStatic)
					{
						return SpreadConversion(Spread_ResolverState::Spread_STATIC, ent->m_flLowerBodyYawTarget());
					}
					else {
						if (res.Spread.LastMovingLBYCanBeUsed)
						{
							return SpreadConversion(Spread_ResolverState::Spread_LASTMOVING, res.Spread.LastMovingLBY);
						}
						else {
							return SpreadConversion(Spread_ResolverState::Spread_NONE, ent->m_flLowerBodyYawTarget());
						}
					}
				}
			}
		}
	}
	else { // Moving in the air
		return SpreadConversion(Spread_ResolverState::Spread_LBY, ent->m_flLowerBodyYawTarget());
	}
}

void SpreadResolver::Main_Update(entity* ent, ResolverPlayer &res, entity* local)
{
	if (Resolver::TargetValid(ent))
	{
		SpreadConversion result = ConversionToResolverState(ent, local, res);
		res.Spread.ResolveMode = result.mode;
		res.Spread.ResolverAngle = result.angle;
	}
}

