#include "Aimbot.h"

#include "MathUtils.h"
#include "RandomStuff.h"

#include "InterfaceManager.h"

#include "checksum_md5.h"

#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )

void NoSpread::NoSpread(entity* local, baseweapon* weapon, CUserCmd* cmd)
{
	int random_seed = MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;

	MathUtils::RandomSeed((random_seed & 255) + 1);

	float s1 = MathUtils::RandomFloat(0.f, 1.f), s2 = MathUtils::RandomFloat(0.f, 2.f * M_PI), s3 = MathUtils::RandomFloat(0.f, 1.f), s4 = MathUtils::RandomFloat(0.f, 2.f * M_PI);

	if (weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER && cmd->buttons & IN_ATTACK2)
	{
		s1 = 1.f - s1 * s1;
		s3 = 1.f - s3 * s3;
	}
	else if (weapon->m_iItemDefinitionIndex() == WEAPON_NEGEV && weapon->m_flRecoilIndex() < 3.f)
	{
		for (auto i = 3; i > weapon->m_flRecoilIndex(); i--)
		{
			s1 *= s1;
			s3 *= s3;
		}

		s1 = 1.f - s1;
		s3 = 1.f - s3;
	}

	auto random_spread = s1 * weapon->GetSpread();
	auto cone = s3 * (weapon->GetInaccuracy() + weapon->GetSpread());

	auto spread = Vector(sin(s2) * random_spread + sin(s4) * cone, cos(s2) * random_spread + cos(s4) * cone, 0.f);

	cmd->viewangles.x += RAD2DEG(atan(spread.Length2D()));
	cmd->viewangles.z = -RAD2DEG(atan2(spread.y, spread.x));
}