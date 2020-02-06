#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Vector.h"
#include "ClientClass.h"
#include "RandomStuff.h"
#include "netvars.h"
#include "MathUtils.h"
#include "ModelRender.h"
#include "BoneAccessor.h"

#include <algorithm>
#include <iostream>

struct model_t;
struct mstudiobbox_t
{
	int     bone;
	int     group;
	Vector  bbmin;
	Vector  bbmax;
	int     szhitboxnameindex;
	int32_t pad[3];
	float m_flRadius;
	int32_t pad2[4];

	char* GetHitboxName(void)
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((char*)this) + szhitboxnameindex;
	}
};
struct mstudiohitboxset_t
{
	int    sznameindex;
	int    numhitboxes;
	int    hitboxindex;

	const char* GetName()
	{
		if (!sznameindex) return nullptr;
		return (const char*)((uint8_t*)this + sznameindex);
	}

	mstudiobbox_t* GetHitbox(int i)
	{
		if (i > numhitboxes) return nullptr;
		return (mstudiobbox_t*)((uint8_t*)this + hitboxindex) + i;
	}
};
struct mstudiobone_t
{
	int                                     sznameindex;
	inline char * const GetName(void) const { return ((char *)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vector                          pos;
	float                           quat[4];
	Vector                          rot;
	Vector                          posscale;
	Vector                          rotscale;

	matrix3x4_t                     poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void *           GetProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char * const GetSurfaceProps(void) const { return ((char *)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};
struct weaponinfo_t 
{
	char pad_0000[4]; //0x0000
	char* m_szWeaponName; //0x0004
	char pad_0008[12]; //0x0008
	int m_iMaxClip; //0x0014
	char pad_0018[12]; //0x0018
	int m_iMaxReservedAmmo; //0x0024
	char pad_0028[4]; //0x0028
	char* szWorldModel; //0x002C
	char* szViewModel; //0x0030
	char* szDropedModel; //0x0034
	char pad_0038[4]; //0x0038
	char* N00000984; //0x003C
	char pad_0040[56]; //0x0040
	char* szEmptySound; //0x0078
	char pad_007C[4]; //0x007C
	char* szBulletType; //0x0080
	char pad_0084[4]; //0x0084
	char* szHudName; //0x0088
	char* szWeaponName; //0x008C
	char pad_0090[60]; //0x0090
	int WeaponType; //0x00CC
	int iWeaponPrice; //0x00D0
	int iKillAward; //0x00D4
	char* szAnimationPrefex; //0x00D8
	float flCycleTime; //0x00DC
	float flCycleTimeAlt; //0x00E0
	float flTimeToIdle; //0x00E4
	float flIdleInterval; //0x00E8
	bool bFullAuto; //0x00EC
	char pad_00ED[3]; //0x00ED
	int m_iWeaponDamage; //0x00F0
	float m_flArmorRatio; //0x00F4
	int iBullets; //0x00F8
	float m_flPenetration; //0x00FC
	float flFlinchVelocityModifierLarge; //0x0100
	float flFlinchVelocityModifierSmall; //0x0104
	float m_flWeaponRange; //0x0108
	float m_flRangeModifier; //0x010C
	float m_flThrowVelocity; //0x0110
	int iCrosshairMinDistance; //0x012C
	float m_flMaxSpeed; //0x0130
	float flMaxPlayerSpeedAlt; //0x0134
	char pad_0138[4]; //0x0138
	float flSpread; //0x013C
	float flSpreadAlt; //0x0140
	float flInaccuracyCrouch; //0x0144
	float flInaccuracyCrouchAlt; //0x0148
	float flInaccuracyStand; //0x014C
	float flInaccuracyStandAlt; //0x0150
	float flInaccuracyJumpIntial; //0x0154
	float flInaccuracyJump; //0x0158
	float flInaccuracyJumpAlt; //0x015C
	float flInaccuracyLand; //0x0160
	float flInaccuracyLandAlt; //0x0164
	float flInaccuracyLadder; //0x0168
	float flInaccuracyLadderAlt; //0x016C
	float flInaccuracyFire; //0x0170
	float flInaccuracyFireAlt; //0x0174
	float flInaccuracyMove; //0x0178
	float flInaccuracyMoveAlt; //0x017C
	float flInaccuracyReload; //0x0180
	int iRecoilSeed; //0x0184
	float flRecoilAngle; //0x0188
	float flRecoilAngleAlt; //0x018C
	float flRecoilVariance; //0x0190
	float flRecoilAngleVarianceAlt; //0x0194
	float flRecoilMagnitude; //0x0198
	float flRecoilMagnitudeAlt; //0x019C
	float flRecoilMagnatiudeVeriance; //0x01A0
	float flRecoilMagnatiudeVerianceAlt; //0x01A4
	float flRecoveryTimeCrouch; //0x01A8
	float flRecoveryTimeStand; //0x01AC
	float flRecoveryTimeCrouchFinal; //0x01B0
	float flRecoveryTimeStandFinal; //0x01B4
	int iRecoveryTransititionStartBullet; //0x01B8
	int iRecoveryTransititionEndBullet; //0x01BC
	bool bUnzoomAfterShot; //0x01C0
	char pad_01C1[31]; //0x01C1
	char* szWeaponClass; //0x01E0
	char pad_01E4[56]; //0x01E4
	float flInaccuracyPitchShift; //0x021C
	float flInaccuracySoundThreshold; //0x0220
	float flBotAudibleRange; //0x0224
	char pad_0228[12]; //0x0228
	bool bHasBurstMode; //0x0234
}; //Size: 0x0440
struct varmapping_t
{
	varmapping_t()
	{
		InterpolatedEntries = 0;
	}

	char	  Entries[20];
	int       InterpolatedEntries;
	float     lastInterpolationTime;
};
struct RenderableInstance_t;

class playerinfo
{
public:
	char pad_0x0000[0x10]; //0x0000
	char name[64]; //0x0010 
	char pad_0x0050[0x40]; //0x0050
	__int32 userID; //0x0090 
	char guid[32]; //0x0094 
	char pad_0x00B4[0x180]; //0x00B4
};
class studiohdr_t
{
public:
	__int32 id;                     //0x0000 
	__int32 version;                //0x0004 
	long    checksum;               //0x0008 
	char    szName[64];             //0x000C 
	__int32 length;                 //0x004C 
	Vector  vecEyePos;              //0x0050 
	Vector  vecIllumPos;            //0x005C 
	Vector  vecHullMin;             //0x0068 
	Vector  vecHullMax;             //0x0074 
	Vector  vecBBMin;               //0x0080 
	Vector  vecBBMax;               //0x008C 
	__int32 flags;                  //0x0098 
	__int32 numbones;               //0x009C 
	__int32 boneindex;              //0x00A0 
	__int32 numbonecontrollers;     //0x00A4 
	__int32 bonecontrollerindex;    //0x00A8 
	__int32 numhitboxsets;          //0x00AC 
	__int32 hitboxsetindex;         //0x00B0 
	__int32 numlocalanim;           //0x00B4 
	__int32 localanimindex;         //0x00B8 
	__int32 numlocalseq;            //0x00BC 
	__int32 localseqindex;          //0x00C0 
	__int32 activitylistversion;    //0x00C4 
	__int32 eventsindexed;          //0x00C8 
	__int32 numtextures;            //0x00CC 
	__int32 textureindex;           //0x00D0

	mstudiohitboxset_t* GetHitboxSet(int i)
	{
		if (i > numhitboxsets) return nullptr;
		return (mstudiohitboxset_t*)((uint8_t*)this + hitboxsetindex) + i;
	}
	mstudiobone_t* GetBone(int i)
	{
		if (i > numbones) return nullptr;
		return (mstudiobone_t*)((uint8_t*)this + boneindex) + i;
	}
};//Size=0x00D4
class animationlayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
};
class IClientRenderable;
class IClientNetworkable;
class IClientUnknown;
class IClientThinkable;
class C_BaseEntity;
class IClientAlphaProperty;
class QAngle;
class bf_read;
class CClientThinkHandlePtr;
class entity;

typedef CClientThinkHandlePtr* ClientThinkHandle_t;
typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8_t;

enum MoveType_t
{
	MOVETYPE_NONE = 0,			// never moves
	MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK,				// Player only - moving on the ground
	MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,				// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
	MOVETYPE_PUSH,				// no clip to world, push and crush
	MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER,			// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics
};
enum ItemDefinitionIndex
{
	WEAPON_NONE = 0,

	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_HUNTSMAN = 509,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	STUDDED_BLOODHOUND_GLOVES = 5027,
	T_GLOVES = 5028,
	CT_GLOVES = 5029,
	SPORTY_GLOVES = 5030,
	SLICK_GLOVES = 5031,
	LEATHER_HANDWRAPS = 5032,
	MOTORCYCLE_GLOVES = 5033,
	SPECIALIST_GLOVES = 5034,
	STUDDED_HYDRA_GLOVES = 5035
/*

	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_HUNTSMAN = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER*/

	//WEAPON_DEAGLE = 1,
	//WEAPON_ELITE = 2,
	//WEAPON_FIVESEVEN = 3,
	//WEAPON_GLOCK = 4,
	//WEAPON_AK47 = 7,
	//WEAPON_AUG = 8,
	//WEAPON_AWP = 9,
	//WEAPON_FAMAS = 10,
	//WEAPON_G3SG1 = 11,
	//WEAPON_GALILAR = 13,
	//WEAPON_M249 = 14,
	//WEAPON_M4A1 = 16,
	//WEAPON_MAC10 = 17,
	//WEAPON_P90 = 19,
	//WEAPON_UMP45 = 24,
	//WEAPON_XM1014 = 25,
	//WEAPON_BIZON = 26,
	//WEAPON_MAG7 = 27,
	//WEAPON_NEGEV = 28,
	//WEAPON_SAWEDOFF = 29,
	//WEAPON_TEC9 = 30,
	//WEAPON_TASER = 31,
	//WEAPON_HKP2000 = 32,
	//WEAPON_MP7 = 33,
	//WEAPON_MP9 = 34,
	//WEAPON_NOVA = 35,
	//WEAPON_P250 = 36,
	//WEAPON_SCAR20 = 38,
	//WEAPON_SG553 = 39,
	//WEAPON_SSG08 = 40,
	//WEAPON_KNIFE = 42,
	//WEAPON_FLASHBANG = 43,
	//WEAPON_HEGRENADE = 44,
	//WEAPON_SMOKEGRENADE = 45,
	//WEAPON_MOLOTOV = 46,
	//WEAPON_DECOY = 47,
	//WEAPON_INCGRENADE = 48,
	//WEAPON_C4 = 49,
	//WEAPON_KNIFE_T = 59,
	//WEAPON_M4A1_SILENCER = 60,
	//WEAPON_USP_SILENCER = 61,
	//WEAPON_CZ75A = 63,
	//WEAPON_REVOLVER = 64,
	//WEAPON_TAGRENADE = 68,
	//WEAPON_KNIFE_BAYONET = 500,
	//WEAPON_KNIFE_FLIP = 505,
	//WEAPON_KNIFE_GUT = 506,
	//WEAPON_KNIFE_KARAMBIT = 507,
	//WEAPON_KNIFE_M9_BAYONET = 508,
	//WEAPON_KNIFE_HUNTSMAN = 509,
	//WEAPON_KNIFE_FALCHION = 512,
	//WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	//WEAPON_KNIFE_BUTTERFLY = 515,
	//WEAPON_KNIFE_PUSH = 516,
	//GLOVE_STUDDED_BLOODHOUND = 5027,
	//GLOVE_CT_SIDE = 5028,
	//GLOVE_T_SIDE = 5029,
	//GLOVE_SPORTY = 5030,
	//GLOVE_SLICK = 5031,
	//GLOVE_LEATHER_WRAP = 5032,
	//GLOVE_MOTORCYCLE = 5033,
	//GLOVE_SPECIALIST = 5034,
	//GLOVE_HYDRA = 5035,
};
enum ShadowType_t
{
	SHADOWS_NONE = 0,
	SHADOWS_SIMPLE,
	SHADOWS_RENDER_TO_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,	// the shadow is always changing state
	SHADOWS_RENDER_TO_DEPTH_TEXTURE,
};
enum WeaponType_t
{
	WEAPON_TYPE_PISTOL,
	WEAPON_TYPE_SNIPER,
	WEAPON_TYPE_RIFLE,
	WEAPON_TYPE_SUBMACHINE,
	WEAPON_TYPE_SHOTGUN,
	WEAPON_TYPE_GRENADE,
	WEAPON_TYPE_MACHINEGUN,
	WEAPON_TYPE_KNIFE,
	WEAPON_TYPE_OTHER
};

class IClientRenderable
{
public:
	// Gets at the containing class...
	virtual IClientUnknown*            GetIClientUnknown() = 0;

	// Data accessors
	virtual Vector const&              GetRenderOrigin(void) = 0;
	virtual QAngle const&              GetRenderAngles(void) = 0;
	virtual bool                       ShouldDraw(void) = 0;
	virtual int                        GetRenderFlags(void) = 0; 
	virtual void                       Unknown0(void) const {}
	virtual ClientShadowHandle_t       GetShadowHandle() const = 0;

	// Used by the leaf system to store its render handle.
	virtual ClientRenderHandle_t&      RenderHandle() = 0;

	// Render
	virtual const model_t*             GetModel() const = 0;
	virtual int                        DrawModel(int flags, RenderableInstance_t &instance) = 0;

	// Get the body parameter
	virtual int                        GetBody() = 0;

	// Determine the color modulation amount
	virtual void                       GetColorModulation(float* color) = 0;

	// Returns false if the entity shouldn't be drawn due to LOD. 
	// (NOTE: This is no longer used/supported, but kept in the vtable for backwards compat)
	virtual bool                       LODTest() = 0;

	// Call this to get the current bone transforms for the model.
	// currentTime parameter will affect interpolation
	// nMaxBones specifies how many matrices pBoneToWorldOut can hold. (Should be greater than or
	// equal to studiohdr_t::numbones. Use MAXSTUDIOBONES to be safe.)
	virtual bool                       SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                       SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) = 0;
	virtual void                       DoAnimationEvents(void) = 0;

	// Return this if you want PVS notifications. See IPVSNotify for more info.	
	// Note: you must always return the same value from this function. If you don't,
	// undefined things will occur, and they won't be good.
	virtual void*				       GetPVSNotifyInterface() = 0;

	// Returns the bounds relative to the origin (render bounds)
	virtual void                       GetRenderBounds(Vector& mins, Vector& maxs) = 0;

	// returns the bounds as an AABB in worldspace
	virtual void                       GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	
	// These normally call through to GetRenderAngles/GetRenderBounds, but some entities custom implement them.
	virtual void                       GetShadowRenderBounds(Vector &mins, Vector &maxs, ShadowType_t shadowType) = 0;
};
class IClientNetworkable
{
public:
	// Gets at the containing class...
	virtual IClientUnknown*  GetIClientUnknown() = 0;

	// Called by the engine when the server deletes the entity.
	virtual void             Release() = 0;

	// Supplied automatically by the IMPLEMENT_CLIENTCLASS macros.
	virtual ClientClass*     GetClientClass() = 0;

	// This tells the entity what the server says for ShouldTransmit on this entity.
	// Note: This used to be EntityEnteredPVS/EntityRemainedInPVS/EntityLeftPVS.
	virtual void             NotifyShouldTransmit(int state) = 0;

	//
	// NOTE FOR ENTITY WRITERS: 
	//
	// In 90% of the cases, you should hook OnPreDataChanged/OnDataChanged instead of 
	// PreDataUpdate/PostDataUpdate.
	//
	// The DataChanged events are only called once per frame whereas Pre/PostDataUpdate
	// are called once per packet (and sometimes multiple times per frame).
	//
	// OnDataChanged is called during simulation where entity origins are correct and 
	// attachments can be used. whereas PostDataUpdate is called while parsing packets
	// so attachments and other entity origins may not be valid yet.
	//
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;

	// Called when data is being updated across the network.
	// Only low-level entities should need to know about these.
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;


	virtual void             Unknown0(void) = 0;

	// Objects become dormant on the client if they leave the PVS on the server.
	virtual bool             IsDormant(void) = 0;

	// Ent Index is the server handle used to reference this entity.
	// If the index is < 0, that indicates the entity is not known to the server
	virtual int              GetIndex(void) const = 0;

	// Server to client entity message received
	virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;

	// Get the base pointer to the networked data that GetClientClass->m_pRecvTable starts at.
	// (This is usually just the "this" pointer).
	virtual void*            GetDataTableBasePtr() = 0;

	// Tells the entity that it's about to be destroyed due to the client receiving
	// an uncompressed update that's caused it to destroy all entities & recreate them.
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};
class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetHandle(const HANDLE& handle) = 0;
	virtual const HANDLE& GetHandle() const = 0;
};
class ICollideable
{
public:
	virtual IHandleEntity*      GetEntityHandle() = 0;
	virtual const Vector&       OBBMins() const = 0;
	virtual const Vector&       OBBMaxs() const = 0;
};
class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable*              GetCollideable() = 0;
	virtual IClientNetworkable*        GetClientNetworkable() = 0;
	virtual IClientRenderable*         GetClientRenderable() = 0;
	virtual entity*					   GetIClientEntity() = 0;
	virtual C_BaseEntity*              GetBaseEntity() = 0;
	virtual IClientThinkable*          GetClientThinkable() = 0;
	virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};
class IClientThinkable
{
public:
	virtual IClientUnknown*     GetIClientUnknown() = 0;
	virtual void                ClientThink() = 0;
	virtual ClientThinkHandle_t GetThinkHandle() = 0;
	virtual void                SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void                Release() = 0;
};

class econitemview
{
public:
	short& m_iItemDefinitionIndex()
	{
		return *(short*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex"));
	}
	int m_iItemIDLow()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_iItemIDLow"));
	}
	int m_iItemIDHigh()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_iItemIDHigh"));
	}
};
class attributableitem // Credits to MarkHC | https://github.com/MarkHC/CSGOSimple/blob/d0d934c25fcc08d8c0adcaa0dfd01d2986c657e1/CSGOSimple/valve_sdk/csgostructs.hpp#L222
{
public:
	int& m_nFallbackPaintKit()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit"));
	}
	int& m_nFallbackSeed()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed"));
	}
	float& m_flFallbackWear()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear"));
	}
	int& m_iEntityQuality()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseAttributableItem", "m_iEntityQuality"));
	}

	econitemview& m_Item()
	{
		// Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
		// A real fix for this requires a rewrite of the netvar manager
		return *(econitemview*)this;
	}
};

class baseweapon : public attributableitem
{
public:
	float m_flNextPrimaryAttack()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack"));
	}
	int m_iClip1()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1"));
	}
	bool m_bPinPulled()
	{
		return *(bool*)((DWORD)this + netvars->GetOffset("DT_BaseCSGrenade", "m_bPinPulled"));
	}
	float m_flRecoilIndex()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_WeaponCSBase", "m_flRecoilIndex"));
	}
	float m_flThrowStrength()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_BaseCSGrenade", "m_flThrowStrength"));
	}
	HANDLE m_hOwnerEntity()
	{
		return *(HANDLE*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_hOwnerEntity"));
	}
	int& m_iViewModelIndex()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex"));
	}
	int& m_iWorldModelIndex()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex"));
	}
	short m_iItemDefinitionIndex()
	{
		if (this)
		{
			return this->m_Item().m_iItemDefinitionIndex();
		}
		return 0;
	}

	void SetModelIndex(int index)
	{
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		vfunction<oSetModelIndex>(this, 75)(this, index);
	}

	float GetPostponeFireReadyTime()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime"));
	}

	WeaponType_t GetWeaponType()
	{
		if (this)
		{
			static std::vector<int> Pistols = { WEAPON_DEAGLE,WEAPON_CZ75A,WEAPON_ELITE,WEAPON_USP_SILENCER,WEAPON_P250,WEAPON_HKP2000,WEAPON_TEC9,WEAPON_REVOLVER,WEAPON_FIVESEVEN,WEAPON_GLOCK };
			if (std::find(Pistols.begin(), Pistols.end(), m_iItemDefinitionIndex()) != Pistols.end())
			{
				return WEAPON_TYPE_PISTOL;
			}
			static std::vector<int> Grenades = { WEAPON_FLASHBANG, WEAPON_TAGRENADE, WEAPON_SMOKEGRENADE, WEAPON_HEGRENADE, WEAPON_INCGRENADE, WEAPON_MOLOTOV, WEAPON_DECOY };
			if (std::find(Grenades.begin(), Grenades.end(), m_iItemDefinitionIndex()) != Grenades.end())
			{
				return WEAPON_TYPE_GRENADE;
			}
			static std::vector<int> SniperWeapons = { WEAPON_AWP, WEAPON_SSG08, WEAPON_G3SG1, WEAPON_SCAR20 };
			if (std::find(SniperWeapons.begin(), SniperWeapons.end(), m_iItemDefinitionIndex()) != SniperWeapons.end())
			{
				return WEAPON_TYPE_SNIPER;
			}
			static std::vector<int> KnifeWeapons = { WEAPON_KNIFE_CSS, WEAPON_KNIFE_GHOST, WEAPON_KNIFEGG, WEAPON_KNIFE_GYPSY_JACKKNIFE, WEAPON_KNIFE_STILETTO, WEAPON_KNIFE_URSUS, WEAPON_KNIFE_WIDOWMAKER, WEAPON_KNIFE, WEAPON_BAYONET, WEAPON_KNIFE_SURVIVAL_BOWIE, WEAPON_KNIFE_BUTTERFLY,WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GUT,WEAPON_KNIFE_HUNTSMAN,WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET,WEAPON_KNIFE_PUSH, WEAPON_KNIFE_T, };
			if (std::find(KnifeWeapons.begin(), KnifeWeapons.end(), m_iItemDefinitionIndex()) != KnifeWeapons.end())
			{
				return WEAPON_TYPE_KNIFE;
			}
			static std::vector<int> RifleWeapons = { WEAPON_AK47, WEAPON_AUG, WEAPON_FAMAS, WEAPON_GALILAR, WEAPON_M4A1, WEAPON_M4A1_SILENCER, WEAPON_SG556 };
			if (std::find(RifleWeapons.begin(), RifleWeapons.end(), m_iItemDefinitionIndex()) != RifleWeapons.end())
			{
				return WEAPON_TYPE_RIFLE;
			}
			static std::vector<int> ShotgunWeapons = { WEAPON_MAG7,WEAPON_NOVA, WEAPON_SAWEDOFF, WEAPON_XM1014 };
			if (std::find(ShotgunWeapons.begin(), ShotgunWeapons.end(), m_iItemDefinitionIndex()) != ShotgunWeapons.end())
			{
				return WEAPON_TYPE_SHOTGUN;
			}
			static std::vector<int> MachinegunWeapons = { WEAPON_M249, WEAPON_NEGEV };
			if (std::find(MachinegunWeapons.begin(), MachinegunWeapons.end(), m_iItemDefinitionIndex()) != MachinegunWeapons.end())
			{
				return WEAPON_TYPE_MACHINEGUN;
			}
			static std::vector<int> SubmachinegunWeapons = { WEAPON_MP7, WEAPON_MP9, WEAPON_BIZON, WEAPON_MAC10, WEAPON_P90, WEAPON_UMP45 };
			if (std::find(SubmachinegunWeapons.begin(), SubmachinegunWeapons.end(), m_iItemDefinitionIndex()) != SubmachinegunWeapons.end())
			{
				return WEAPON_TYPE_SUBMACHINE;
			}
		}
		return WEAPON_TYPE_OTHER;
	}

	bool m_bIsScopedWeapon() {
		if (this)
		{
			WeaponType_t weapon_type = GetWeaponType();
			if (weapon_type == WEAPON_TYPE_SNIPER || m_iItemDefinitionIndex() == WEAPON_AUG || m_iItemDefinitionIndex() == WEAPON_SG556)
			{
				return true;
			}
		}
		return false;
	}
	bool IsInvalidWeapon() {
		if (this)
		{
			WeaponType_t weapon_type = GetWeaponType();
			if (weapon_type == WEAPON_TYPE_KNIFE || weapon_type == WEAPON_TYPE_GRENADE || weapon_type == WEAPON_TYPE_OTHER)
			{
				return true;
			}
		}
		return false;
	}

	void UpdateAccuracyPenalty() {
		typedef void(__thiscall* original)(void*);
		vfunction<original>(this, 479)(this);
	}
	float GetSpread()
	{
		typedef float(__thiscall* original)(void*);
		return vfunction<original>(this, 448)(this);
	}
	float GetInaccuracy() 
	{
		typedef float(__thiscall* original)(void*);
		return vfunction<original>(this, 478)(this);
	}
	weaponinfo_t* GetCSWpnData();
};

class baseviewmodel
{
public:
	int m_nSequence()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseViewModel", "m_nSequence"));
	}

	void SendViewModelMatchingSequence(int sequence)
	{
		typedef  void(__thiscall* OriginalFn)(void*, int);
		return vfunction<OriginalFn>(this, 241)(this, sequence);
	}
};

class collisionproperty
{
public:
	Vector m_vecMins()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_vecMins"));
	}
	Vector m_vecMaxs()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_vecMaxs"));
	}
};

class entity;

class animationstate
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	entity* pBaseEntity; //0x60
	baseweapon* pActiveWeapon; //0x64
	baseweapon* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float m_velocity; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
};

class ragdoll
{
public:
	Vector m_ragPos()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_Ragdoll", "m_ragPos[0]"));
	}
};

class entity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	float m_flNextAttack()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_flNextAttack"));
	}
	uint32_t& m_nEffects()
	{
		uint32_t* target_effects = (uint32_t*)((uintptr_t)this + 0x00EC);
		return *target_effects;
	}
	int m_iHealth()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_iHealth"));
	}
	int& m_nTickBase()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_nTickBase"));
	}
	int m_iTeamNum()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_iTeamNum"));
	}
	int m_ArmorValue()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_ArmorValue"));
	}
	int& m_fFlags()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_fFlags"));
	}
	int m_iShotsFired()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_iShotsFired"));
	}
	int m_bClientSideAnimation()
	{
		return *(int*)((DWORD)this + netvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation"));
	}
	int m_moveType()
	{
		return *(int*)((DWORD)this + 0x258);
	}
	int* m_fFlags_ptr()
	{
		return (int*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_fFlags"));
	}

	bool m_bHasHelmet()
	{
		return *(bool*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_bHasHelmet"));
	}
	bool m_bGunGameImmunity()
	{
		return *(bool*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity"));
	}
	bool m_bIsScoped()
	{
		return *(bool*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_bIsScoped"));
	}
	bool m_bHasHeavyArmor()
	{
		return *(bool*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor"));
	}

	float m_flLowerBodyYawTarget()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget"));
	}
	float m_flDuckAmount()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_flDuckAmount"));
	}
	float m_flSimulationTime()
	{
		return *(float*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_flSimulationTime"));
	}

	Vector m_vecVelocity()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_vecVelocity[0]"));
	}
	Vector m_vecOrigin()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_vecOrigin"));
	}
	Vector m_vecViewOffset()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_vecViewOffset[0]"));
	}
	Vector m_angEyeAngles()
	{
		return *(Vector*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_angEyeAngles[0]"));
	}
	Vector* m_aimPunchAngle()
	{
		return (Vector*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_aimPunchAngle"));
	}
	Vector* m_viewPunchAngle()
	{
		return (Vector*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_viewPunchAngle"));
	}
	Vector* m_angEyeAngles_ptr()
	{
		return (Vector*)((DWORD)this + netvars->GetOffset("DT_CSPlayer", "m_angEyeAngles[0]"));
	}

	HANDLE m_hViewModel()
	{
		return *(HANDLE*)((DWORD)this + netvars->GetOffset("DT_BasePlayer", "m_hViewModel[0]"));
	}
	HANDLE m_hActiveWeapon()
	{
		return *(HANDLE*)((DWORD)this + netvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon"));
	}

	const matrix3x4_t& m_rgflCoordinateFrame()
	{
		return *(matrix3x4_t*)((DWORD)this + (netvars->GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30) );
	}

	varmapping_t* varmapping()
	{
		return (varmapping_t*)((DWORD)this + 0x24);
	}

	collisionproperty* m_Collision()
	{
		return (collisionproperty*)((DWORD)this + netvars->GetOffset("DT_BaseEntity", "m_Collision"));
	}

	Vector GetRagdollPos();

	float tick_base;
	float extrapolation_simulationtime;

	float GetCorrectedTickBase()
	{
		return tick_base;
	}

	float GetOldSimulationTime();

	float GetServerSimulationTime(); // Credits to @Paw on UC
	float GetAnimTime()
	{
		return *(float*)((DWORD)this + 0x025C);
	}

	animationlayer& GetAnimOverlay(int idx)
	{
		if (idx <= GetNumAnimOverlays())
		{
			return (*(animationlayer**)((DWORD)this + 0x2980))[idx]; // AnimOverlays -> ((DWORD)this + 10608)
		}
	}

	bool RebuiltSetupBones(matrix3x4_t *boneOut, int boneMask);
	void SetAbsAngles(Vector angles);
	void SetAbsOrigin(Vector origin);
	void SetObserverMode(int i);
	void DisableInterpolation();
	void InvalidateOcclusion();

	bool IsPlayer();
	bool IsVisible(entity* local);
	bool GetHitboxPosition(int Hitbox, Vector &end);
	bool IsVisiblePosition(Vector pos, entity* ent);
	bool IsVisibleHitBoxPosition(Vector pos, int hitbox, entity* local);

	int GetPing();

	animationstate* GetAnimationState();
	CBoneAccessor* m_BoneAccessor();
	CStudioHdr* GetModelPtr();

	float GetSpawnTime()
	{
		float spawn_time = *(float*)((DWORD)this + 0xA2A0);
		return spawn_time;
	}

	int LookupPoseParameter(char* name);
	float& GetPoseParameters(int idx)
	{
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + netvars->GetOffset("DT_BaseAnimating", "m_flPoseParameter") + sizeof(float) * idx);
	}


	int GetSequenceActivity(int sequence);
	int GetNumAnimOverlays()
	{
		return 15;
	}
	int GetNumHitBoxes()
	{
		return 15;
	}
	void UpdateIKLocks(float currentTime)
	{
		typedef void(__thiscall *original)(void*, float);
		vfunction<original>(this, 187)(this, currentTime);
	}
	void CalculateIKLocks(float currentTime)
	{
		typedef void(__thiscall *original)(void*, float);
		vfunction<original>(this, 188)(this,currentTime);
	}
	void BuildTransformations(CStudioHdr* hdr, Vector* pos, Quaternion *q, const matrix3x4_t& cameraTransform, int32_t boneMask, byte* boneComputed)
	{
		typedef void(__thiscall *o_BuildTransformations)(void*, CStudioHdr*, Vector*, Quaternion*, const matrix3x4_t&, int32_t, byte*);
		vfunction<o_BuildTransformations>(this, 185)(this, hdr, pos, q, cameraTransform, boneMask, boneComputed);
	}
	void StandardBlendingRules(CStudioHdr *hdr, Vector *pos, Quaternion *q, float_t curtime, int32_t boneMask)
	{
		typedef void(__thiscall *o_StandardBlendingRules)(void*, CStudioHdr*, Vector*, Quaternion*, float_t, int32_t);
		vfunction<o_StandardBlendingRules>(this, 201)(this, hdr, pos, q, curtime, boneMask);
	}
	int GetMaxHealth()
	{
		typedef int(__thiscall* original)(void*);
		return vfunction<original>(this, 121)(this);
	}
	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall *original)(void*);
		vfunction<original>(this, 223)(this);
	}

	int OrigInterpEntries;

	Vector GetBonePosition(int bone);
	matrix3x4_t* GetBonePositionMatrix(int bone);
	Vector GetEyePosition();

	Vector& GetAbsOrigin()
	{
		typedef Vector&(__thiscall *original)(void*);
		return vfunction<original>(this, 10)(this);
	}
	Vector& GetAbsAngles()
	{
		typedef Vector&(__thiscall *original)(void*);
		return vfunction<original>(this, 11)(this);
	}

	baseweapon* GetWeapon();
};

#endif