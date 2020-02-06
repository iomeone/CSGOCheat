#pragma once

#include "Vector.h"
#include "RandomStuff.h"
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
namespace MathUtils
{
	void MatrixAngles(const matrix3x4_t &matrix, Vector &q, Vector &pos);
	void VectorAngles(const Vector &forward, const Vector &pseudoup, Vector &angles);
	void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	float DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along = NULL, Vector *pointOnRay = NULL);
	float degrees_to_radians(float deg);
	float radians_to_degrees(float radians);
	Vector get_rotated_position(Vector start, const float rotation, const float distance);
	void AngleMatrix(const Vector &angles, matrix3x4_t& matrix);
	void AngleVectors(const Vector &angles, Vector *forward);
	void VectorTransform(const Vector in1, const float in2[3][4], Vector &out);
	void VectorAngles(Vector forward, Vector &angles);
	void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	void Normalize(Vector &vIn, Vector &vOut);
	float DotProduct(const Vector v1, const Vector v2);
	void AntiUntrusted_ClampAngles(Vector &angle);
	float RandomFloat(float min, float max);
	int RandomInt(int min, int max);
	void RandomSeed(unsigned int seed);
	int GetRandom(int min, int max);
	void AngleMatrix(const Vector &angles, const Vector &position, matrix3x4_t& matrix);
	void MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out);
}