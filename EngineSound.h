#pragma once

#include "UtlVector.h"

typedef void* FileNameHandle_t;

struct SndInfo_t
{
	int m_nGuid;
	FileNameHandle_t m_filenameHandle;
	int m_nSoundSource;
	int m_nChannel;

	int m_nSpeakerEntity;
	float m_flVolume;
	float m_flLastSpatializedVolume;

	float m_flRadius;
	int m_nPitch;
	Vector *m_pOrigin;
	Vector *m_pDirection;

	bool m_bUpdatePositions;
	bool m_bIsSentence;
	bool m_bDryMix;
	bool m_bSpeaker;
	bool m_bSpecialDSP;
	bool m_bFromServer;
};

class IEngineSound
{
public:
	void GetActiveSounds(CUtlVector<SndInfo_t>& sndlist)
	{
		typedef void(__thiscall* oGetActiveSounds)(PVOID, CUtlVector<SndInfo_t>& sndlist);
		vfunction<oGetActiveSounds>(this, 19)(this, sndlist);
	}
};