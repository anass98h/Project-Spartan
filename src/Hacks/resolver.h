#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/pstring.h"
#include "../Utils/entity.h"
#include "../SDK/CTickRecord.h"
#include <random>





    #define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / globalVars->interval_per_tick ) )
    #define TICKS_TO_TIME( t )		( globalVars->interval_per_tick * ( t ) )

   struct LagRecord
{
	Vector m_vecVelocity;
	Vector m_vAbsOrigin;
	Vector m_vecAngles;
	Vector m_eyeAngles;
	int m_iTargetID;
	int m_nSequence;
	int m_iWantedTick;
	int m_iBacktrackedTick;
	float m_fSimulationTime;
	float m_flCycle;
	float m_flPoseParameter[24];
	float m_flLowerBodyYawTarget;
	unsigned char flags;
	bool m_bIsBreakingLagComp;
	bool m_bIsFixed;
	Vector headSpot;

	LagRecord()
	{
		m_iTargetID = -1;
		m_fSimulationTime = -1;
		m_iWantedTick = -1;
		m_bIsBreakingLagComp = false;
		m_bIsFixed = false;
		m_iBacktrackedTick = -1;
		m_vAbsOrigin = Vector(0, 0, 0);
		m_flCycle = -1;
		m_nSequence = -1;
		m_flLowerBodyYawTarget = 0;
		m_flPoseParameter[24] = 0;
		m_vecAngles = Vector(0, 0, 0);
		flags = -1;
		m_vecVelocity = Vector(0, 0, 0);
		m_eyeAngles = Vector(0, 0, 0);
		headSpot = Vector(0, 0, 0);
	}

	LagRecord(const LagRecord& src)
	{
		m_flCycle = src.m_flCycle;
		m_nSequence = src.m_nSequence;
		m_iTargetID = src.m_iTargetID;
		m_vecAngles = src.m_vecAngles;
		m_fSimulationTime = src.m_fSimulationTime;
		m_iWantedTick = src.m_iWantedTick;
		m_bIsBreakingLagComp = src.m_bIsBreakingLagComp;
		m_bIsFixed = src.m_bIsFixed;
		m_vAbsOrigin = src.m_vAbsOrigin;
		m_iBacktrackedTick = src.m_iBacktrackedTick;
		flags = src.flags;
		m_flLowerBodyYawTarget = src.m_flLowerBodyYawTarget;
		m_flPoseParameter[24] = src.m_flPoseParameter[24];
		m_vecVelocity = src.m_vecVelocity;
		m_eyeAngles = src.m_eyeAngles;
		headSpot = src.headSpot;
	}
};

struct LagCompensation
{
	LagRecord m_LagRecord[64][11] = {};
	LagRecord m_PrevRecords[64] = {};
       
	 LagCompensation(); 
	void InitLagRecord();
	int  FixTickcount(C_BasePlayer* player);
	bool IsValidTick(float simTime);


	template<class T, class U>
	T clamp(T in, U low, U high);
	float LerpTime();
};
extern LagCompensation* LagComp;



class CResolveInfo {
   
	public:
    	std::deque<CTickRecord> m_sRecords;
    	bool	m_bEnemyShot; //priority
    	bool	m_bLowerBodyYawChanged;
    	bool	m_bBacktrackThisTick;
};

namespace Resolver
{       
    
  
	extern std::vector<int64_t> playerAngleLogs;
	extern std::array<CResolveInfo, 32> m_arrInfos;

	void Hug(C_BasePlayer* Circlebian);
	void FrameStageNotify(ClientFrameStage_t stage);
	void PostFrameStageNotify(ClientFrameStage_t stage);
	void FireGameEvent(IGameEvent* event);
        void CreateMove(CUserCmd* cmd);
	CTickRecord GetShotRecord(C_BasePlayer*);
	
    bool HasStaticRealAngle(int index, float tolerance = 15.f);
    bool HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance = 15.f);
    bool HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance = 15.f);
    bool HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance = 15.f);
    int GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance = 15.f);
    int GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance = 15.f);
    float GetLBYByComparingTicks(const std::deque<CTickRecord>& l);
    float GetDeltaByComparingTicks(const std::deque<CTickRecord>& l);
    bool DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance = 15.f);
    bool LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance = 15.f);
    bool IsEntityMoving(C_BasePlayer* Circlebian);
	bool& LowerBodyYawChanged(C_BasePlayer* Circlebian);
	void StoreVars(C_BasePlayer* Circlebian);
	void StoreVars(C_BasePlayer* Circlebian, QAngle ang, float lby, float simtime, float tick);
	bool& BacktrackThisTick(C_BasePlayer* Circlebian);
}

const inline float GetDelta(float a, float b) {
    return abs(Math::ClampYaw(a - b));
}

const inline float LBYDelta(const CTickRecord& v) {
    return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
    return (GetDelta(a, b) > tolerance);
}
