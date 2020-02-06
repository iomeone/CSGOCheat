#include "MathUtils.h"

#include <Windows.h>

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define Assert( _exp ) ((void)0)

void MathUtils::MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out)
{
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}

void MathUtils::AngleMatrix(const Vector &angles, const Vector &position, matrix3x4_t& matrix)
{
	AngleMatrix(angles, matrix);
	MatrixSetColumn(position, 3, matrix);
}

float MathUtils::DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along, Vector *pointOnRay)
{
	Vector to = pos - rayStart;
	Vector dir = rayEnd - rayStart;

	float length = dir.Normalized().Length();

	float rangeAlong = dir.Dot(to);
	if (along)
		*along = rangeAlong;

	float range;

	if (rangeAlong < 0.0f)
	{
		range = -(pos - rayStart).Length();

		if (pointOnRay)
			*pointOnRay = rayStart;
	}
	else if (rangeAlong > length)
	{
		range = -(pos - rayEnd).Length();

		if (pointOnRay)
			*pointOnRay = rayEnd;
	}
	else
	{
		Vector onRay = rayStart + rangeAlong * dir;
		range = (pos - onRay).Length();

		if (pointOnRay)
			*pointOnRay = onRay;
	}

	return range;
}

void MatrixGetColumn(const matrix3x4_t& in, int column, Vector &out)
{
	out.x = in[0][column];
	out.y = in[1][column];
	out.z = in[2][column];
}

void MathUtils::MatrixAngles(const matrix3x4_t &matrix, Vector &q, Vector &pos)
{
	float trace;
	trace = matrix[0][0] + matrix[1][1] + matrix[2][2] + 1.0f;
	if (trace > 1.0f + FLT_EPSILON)
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion A",1);
		q.x = (matrix[2][1] - matrix[1][2]);
		q.y = (matrix[0][2] - matrix[2][0]);
		q.z = (matrix[1][0] - matrix[0][1]);
	}
	else if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2])
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion B",1);
		trace = 1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2];
		q.x = trace;
		q.y = (matrix[1][0] + matrix[0][1]);
		q.z = (matrix[0][2] + matrix[2][0]);
	}
	else if (matrix[1][1] > matrix[2][2])
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion C",1);
		trace = 1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2];
		q.x = (matrix[0][1] + matrix[1][0]);
		q.y = trace;
		q.z = (matrix[2][1] + matrix[1][2]);
	}
	else
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion D",1);
		trace = 1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1];
		q.x = (matrix[0][2] + matrix[2][0]);
		q.y = (matrix[2][1] + matrix[1][2]);
		q.z = trace;
	}

	MatrixGetColumn(matrix, 3, pos);
}

void SinCos(float a, float* s, float*c)
{
	*s = sin(a);
	*c = cos(a);
}

float MathUtils::radians_to_degrees(float radians)
{
	return radians * (180.f / M_PI);
}

float MathUtils::degrees_to_radians(float deg)
{
	return deg * (M_PI / 180.f);
}

Vector MathUtils::get_rotated_position(Vector start, const float rotation, const float distance)
{
	float rad = degrees_to_radians(rotation);
	start.x += cosf(rad) * distance;
	start.y += sinf(rad) * distance;

	return start;
}

void MathUtils::AngleMatrix(const Vector &angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;

	float crcy = cr*cy;
	float crsy = cr*sy;
	float srcy = sr*cy;
	float srsy = sr*sy;
	matrix[0][1] = sp*srcy - crsy;
	matrix[1][1] = sp*srsy + crcy;
	matrix[2][1] = sr*cp;

	matrix[0][2] = (sp*crcy + srsy);
	matrix[1][2] = (sp*crsy - srcy);
	matrix[2][2] = cr*cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

int MathUtils::GetRandom(int min, int max) {
	int num = (max - min + 1);
	if (num == 0) // To avoid this exception: Integer division by zero.
		return 0;

	return (rand() % num) + min;
}

void MathUtils::RandomSeed(unsigned int seed)
{
	typedef void(*RandomSeed_t)(unsigned int);
	static RandomSeed_t RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	return RandomSeed(seed);
}

int MathUtils::RandomInt(int min, int max)
{
	typedef float(*RandomInt_t)(int, int);
	static RandomInt_t RandomInt = (RandomInt_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomInt");
	return RandomInt(min, max);
}

float MathUtils::RandomFloat(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	static RandomFloat_t RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return RandomFloat(min, max);
}

void MathUtils::AntiUntrusted_ClampAngles(Vector &angle) // Anti-Untrusted clamp
{
	if (angle.y < -180) angle.y += 360;
	if (angle.y > 180) angle.y -= 360;

	if (angle.x > 89) angle.x = 89;
	if (angle.x < -89) angle.x = -89;
	if (angle.y < -180) angle.y = -180;
	if (angle.y > 180) angle.y = 180;

	angle.z = 0;
}

void MathUtils::Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float MathUtils::DotProduct(const Vector v1, const Vector v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
};

void MathUtils::VectorTransform(const Vector in1, const float in2[3][4], Vector &out)
{
	out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}

Vector CrossProducts(const Vector &a, const Vector &b)
{
	return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
void MathUtils::VectorAngles(const Vector &forward, const Vector &pseudoup, Vector &angles)
{
	Vector left = CrossProducts(pseudoup, forward);
	left.NormalizeInPlace();

	float forwardDist = forward.Length2D();

	if (forwardDist > 0.001f)
	{
		angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
		angles.y = atan2f(forward.y, forward.x) * 180 / 3.14159265358979323846f;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f(left.z, upZ) * 180 / 3.14159265358979323846f;
	}
	else
	{
		angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
		angles.y = atan2f(-left.x, left.y) * 180 / 3.14159265358979323846f;
		angles.z = 0;
	}
}

void MathUtils::AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	Assert(s_bMathlibInitialized);

	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}

void MathUtils::AngleVectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

void MathUtils::VectorAngles(Vector forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward[2] == 0 && forward[0] == 0)
	{
		yaw = 0;

		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / PI);

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);

		if (pitch < 0)
			pitch += 360;
	}

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}