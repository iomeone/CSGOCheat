//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef BONE_ACCESSOR_H
#define BONE_ACCESSOR_H
#ifdef _WIN32
#pragma once
#endif

#include <Windows.h>

typedef float matrix3x4_t[3][4];
class C_BaseAnimating;

class CBoneAccessor
{
public:
	CBoneAccessor();
	CBoneAccessor(matrix3x4_t *pBones); // This can be used to allow access to all bones.

	// Initialize.
	void Init(const C_BaseAnimating *pAnimating, matrix3x4_t *pBones);
	void SetBoneArrayForWrite(matrix3x4_t *bone_array);

	// Get bones for read or write access.
	const matrix3x4_t&	GetBone(int iBone) const;
	const matrix3x4_t&	operator[](int iBone) const;

	matrix3x4_t&		GetBoneForWrite(int iBone);
	matrix3x4_t			*GetBoneArrayForWrite() const;

private:
	// Only used in the client DLL for debug verification.
	const C_BaseAnimating *m_pAnimating;

	matrix3x4_t *m_pBones;

	int m_ReadableBones;		// Which bones can be read.
	int m_WritableBones;		// Which bones can be written.
};

inline void CBoneAccessor::SetBoneArrayForWrite(matrix3x4_t *bone_array)
{
	m_pBones = bone_array;
}

inline CBoneAccessor::CBoneAccessor()
{
	m_pAnimating = NULL;
	m_pBones = NULL;
	m_ReadableBones = m_WritableBones = 0;
}

inline CBoneAccessor::CBoneAccessor(matrix3x4_t *pBones)
{
	m_pAnimating = NULL;
	m_pBones = pBones;
}

inline void CBoneAccessor::Init(const C_BaseAnimating *pAnimating, matrix3x4_t *pBones)
{
	m_pAnimating = pAnimating;
	m_pBones = pBones;
}

inline matrix3x4_t *CBoneAccessor::GetBoneArrayForWrite(void) const
{
	return m_pBones;
}

#endif // BONE_ACCESSOR_H