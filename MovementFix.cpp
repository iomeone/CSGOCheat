#include "MovementFix.h"

#include "MathUtils.h"

Vector AimAngles, viewforward, viewright, viewup, aimforward, aimright, aimup;

void MovementFix::start(CUserCmd* cmd)
{
	AimAngles = Vector(0.0f, cmd->viewangles.y, 0.0f);
	MathUtils::AngleVectors(AimAngles, &viewforward, &viewright, &viewup);

}
void MovementFix::end(CUserCmd* cmd, Vector movementvector)
{
	AimAngles = Vector(0.0f, movementvector.y, 0.0f);
	MathUtils::AngleVectors(AimAngles, &viewforward, &viewright, &viewup);

	AimAngles = Vector(0.0f, cmd->viewangles.y, 0.0f);
	MathUtils::AngleVectors(AimAngles, &aimforward, &aimright, &aimup);

	Vector vForwardNormalized;	MathUtils::Normalize(viewforward, vForwardNormalized);
	Vector vRightNormalized;		MathUtils::Normalize(viewright, vRightNormalized);
	Vector vUpNormalized;			MathUtils::Normalize(viewup, vUpNormalized);

	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;

	if (forward > 450)
		forward = 450;
	if (forward < -450)
		forward = -450;

	if (right > 450)
		right = 450;
	if (right < -450)
		right = -450;

	if (up > 450)
		up = 450;
	if (up < -450)
		up = -450;

	cmd->forwardmove = MathUtils::DotProduct(forward * vForwardNormalized, aimforward) + MathUtils::DotProduct(right * vRightNormalized, aimforward) + MathUtils::DotProduct(up * vUpNormalized, aimforward);
	cmd->sidemove = MathUtils::DotProduct(forward * vForwardNormalized, aimright) + MathUtils::DotProduct(right * vRightNormalized, aimright) + MathUtils::DotProduct(up * vUpNormalized, aimright);
	cmd->upmove = MathUtils::DotProduct(forward * vForwardNormalized, aimup) + MathUtils::DotProduct(right * vRightNormalized, aimup) + MathUtils::DotProduct(up * vUpNormalized, aimup);
}