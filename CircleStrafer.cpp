#include "MiscFunctions.h"

#include "MathUtils.h"
#include "InterfaceManager.h"
#include "Menu.h"
#include "RenderUtils.h"

Vector draw_angles;
Vector objective_vec;

void Misc_Functions::CircleStrafeDraw(entity* local)
{
	Vector chest_hitbox;
	if (local->GetHitboxPosition(Hitboxes::HITBOX_PELVIS, chest_hitbox))
	{
		chest_hitbox.z -= 30;

		Vector W2S_base, W2S_direction;
		Vector angles = draw_angles;
		Vector direction;
		angles.x = 0;

		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = local;
		trace_t trace;


		MathUtils::AngleVectors(angles - Vector(0, 45.f, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles - Vector(0, 90.f, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles - Vector(0, 135, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles - Vector(0, 180.f, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles - Vector(0, 225.f, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles - Vector(0, 270.f, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles - Vector(0, 315.f, 0), &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}

		MathUtils::AngleVectors(angles, &direction);
		direction = chest_hitbox + direction * 100.f;
		ray.Init(chest_hitbox, direction);
		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(trace.endpos, W2S_direction))
		{
			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_direction.x, W2S_direction.y, trace.DidHitWorld() ? Color(255, 0, 0) : Color(0, 255, 0));
		}


		//bool Should_CircleStrafe = false;
		//	bool has_to_turn = false;
		//	float end_multiplier = 0.f;
		//	Vector end_vector;

		//	for (float i = 0.f; i < 400.f; i++)
		//	{
		//		Vector forward;
		//		MathUtils::AngleVectors(angles, &forward);
		//		forward = chest_hitbox + forward * i;
		//	
		//		ray.Init(chest_hitbox, forward);
		//		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		//		if (!trace.DidHitWorld())
		//		{
		//			end_vector = trace.endpos;
		//			end_multiplier = i;
		//			points.push_back(forward);
		//		}
		//		else {
		//			has_to_turn = true;
		//		}
		//	}

		//	Vector end_right;
		//	Vector end_left;

		//	if (has_to_turn)
		//	{
		//		for (float o = 0; o < 100.f; o++)
		//		{
		//			Vector left;
		//			MathUtils::AngleVectors(angles + Vector(0, 90, 0), &left);
		//			left = end_vector + left * o;

		//			ray.Init(end_vector, left);
		//			game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//			if (!trace.DidHitWorld())
		//			{
		//				end_right = trace.endpos;
		//				points.push_back(left);
		//			}
		//		}
		//		for (float o = 0; o < 100.f; o++)
		//		{
		//			Vector left;
		//			MathUtils::AngleVectors(angles - Vector(0, 90, 0), &left);
		//			left = end_vector + left * o;

		//			ray.Init(end_vector, left);
		//			game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//			if (!trace.DidHitWorld())
		//			{
		//				end_left = trace.endpos;
		//				points.push_back(left);
		//			}
		//		}
		//	}

		//
		//	/*if (has_to_turn)
		//	{
		//		bool has_to_turn_again = false;

		//		Vector right;
		//		MathUtils::AngleVectors(angles + Vector(0, 90, 0), &right);
		//		right = chest_hitbox + right * end_multiplier;
		//		ray.Init(chest_hitbox, right);
		//		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//		if (!trace.DidHitWorld())
		//		{
		//			points.push_back(right);
		//			for (float i = end_multiplier; i < (end_multiplier + 200.f); i++)
		//			{
		//				right = chest_hitbox + right * i;
		//				ray.Init(chest_hitbox, right);
		//				game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//				if (!trace.DidHitWorld())
		//				{
		//					points.push_back(right);
		//				}
		//				else {
		//					end_multiplier = i;
		//					has_to_turn_again = true;
		//				}
		//			}
		//		}

		//		if (has_to_turn_again)
		//		{
		//			Vector left;
		//			MathUtils::AngleVectors(angles - Vector(0, 90, 0), &left);
		//			left = chest_hitbox + left * end_multiplier;
		//			ray.Init(chest_hitbox, left);
		//			game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//			if (!trace.DidHitWorld())
		//			{
		//				points.push_back(left);
		//				for (float i = end_multiplier; i < (end_multiplier + 200.f); i++)
		//				{
		//					left = chest_hitbox + left * i;
		//					ray.Init(chest_hitbox, left);
		//					game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//					if (!trace.DidHitWorld())
		//					{
		//						points.push_back(left);
		//					}
		//				}
		//			}
		//		}
		//	}*/


		//if (!Should_CircleStrafe)
		//{
		//	MathUtils::AngleVectors(angles, &forward);
		//	forward = chest_hitbox + forward * 150.f;

		//	Ray_t ray;
		//	ITraceFilter filter;
		//	filter.pSkip = local;
		//	trace_t trace;

		//	ray.Init(chest_hitbox, forward);
		//	game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

		//	if (!trace.DidHitWorld())
		//	{
		//		MathUtils::AngleVectors(angles - Vector(0, 180.f, 0), &forward);
		//		forward = chest_hitbox + forward * 150.f;
		//		ray.Init(chest_hitbox, forward);
		//		game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//		if (!trace.DidHitWorld())
		//		{
		//			MathUtils::AngleVectors(angles - Vector(0, 90, 0), &forward);
		//			forward = chest_hitbox + forward * 150.f;
		//			ray.Init(chest_hitbox, forward);
		//			game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//			if (!trace.DidHitWorld())
		//			{
		//				MathUtils::AngleVectors(angles + Vector(0, 90.f, 0), &forward);
		//				forward = chest_hitbox + forward * 150.f;
		//				ray.Init(chest_hitbox, forward);
		//				game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);
		//				if (!trace.DidHitWorld())
		//				{
		//					Should_CircleStrafe = true;
		//				}
		//			}
		//		}
		//	}
		//}

		//if (!Should_CircleStrafe)
		//{

		//	for (Vector p : points)
		//	{
		//		Vector W2S_base, W2S_forward;
		//		if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(p, W2S_forward))
		//		{
		//			RenderUtils::RenderRect(W2S_forward.x, W2S_forward.y, 5, 5, Color(0, 255, 0));
		//		}
		//	}



		//	Vector W2S_base, W2S_left, W2S_right;
		//	if (RenderUtils::WorldToScreen(chest_hitbox, W2S_base) && RenderUtils::WorldToScreen(end_left, W2S_left) && RenderUtils::WorldToScreen(end_right, W2S_right))
		//	{
		//		float dist_1 = end_vector.DistToSqr(end_left);
		//		float dist_2 = end_vector.DistToSqr(end_right);

		//		bool left_better = false;

		//		if (dist_1 > dist_2)
		//		{
		//			RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_left.x, W2S_left.y, Color(0, 150, 255));
		//		}
		//		else {
		//			if (dist_1 < dist_2)
		//			{
		//				RenderUtils::RenderLine(W2S_base.x, W2S_base.y, W2S_right.x, W2S_right.y, Color(0, 150, 255));
		//			}
		//		}
		//	}
		//}
		//else {
	
		//}




	}

	Vector W2S_obj;
	if (RenderUtils::WorldToScreen(objective_vec, W2S_obj))
	{
		RenderUtils::RenderRect(W2S_obj.x, W2S_obj.y, 5, 5, Color(255, 150, 0));
	}

}

bool can_change_objective;
float spin_value;

Vector Misc_Functions::CircleStrafe(CUserCmd* cmd, entity* local, Vector angles)
{
	draw_angles = angles;
	if (local && GetAsyncKeyState(MenuControls::m_circlestrafer_bind->GetKey()))
	{
		Vector chest_hitbox;
		if (local->GetHitboxPosition(Hitboxes::HITBOX_PELVIS, chest_hitbox))
		{
			chest_hitbox.z -= 30;
			angles.x = 0;

			Vector W2S_base, W2S_direction;
			Vector direction;

			float velocity = local->m_vecVelocity().Length2D();
			

			Ray_t ray;
			CTraceFilter filter;
			filter.pSkip = local;
			trace_t trace;

			MathUtils::AngleVectors(Vector(0, spin_value, 0), &direction);
			direction = chest_hitbox + direction * 100.f;
			ray.Init(chest_hitbox, direction);
			game_interfaces::Trace->TraceRay(ray, MASK_SOLID, &filter, &trace);

			float factor_wall = (trace.startpos.DistTo(trace.endpos) / 1000.f);
			float vel_factor = velocity / 1000.f;
			clamp(vel_factor, 1, 5);

			spin_value += (vel_factor) + 2;
			

			if (spin_value > 360.f)
			{
				spin_value = 0.f;
			}
			
			Vector set = Vector(0, spin_value, 0);
			MathUtils::AntiUntrusted_ClampAngles(set);
			angles = set;

			cmd->forwardmove = 450.f;
		}
	}
	else {
		spin_value = angles.y;
	can_change_objective = true;	
	}
	return angles;
}