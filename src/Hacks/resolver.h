#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/pstring.h"
#include "../Utils/entity.h"
#include "../SDK/CTickRecord.h"
#include <random>


#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / globalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )        ( globalVars->interval_per_tick * ( t ) )


struct StoredNetvars {
    float lby;
    Vector origin;
    Vector min;
    Vector max;
    int flags;
    float simulationTime;
    QAngle eyeAngles;
    //std::array<float, 24> poseParam[24];
    int sequence;
    float cycle;

    Vector velocity;

    bool lbyupdate;
};

class CResolveInfo {

public:
    std::deque<CTickRecord> m_sRecords;
    bool m_bEnemyShot; //priority
    bool m_bLowerBodyYawChanged;
    bool m_bBacktrackThisTick;
};

class LagComp {
private:
    void Store(StoredNetvars *dest, C_BasePlayer *player);
    void Restore(StoredNetvars *src, C_BasePlayer *player);
public:
    std::vector<StoredNetvars> vecLagRecord[64];
    StoredNetvars *pRecordRollback[64];

    float GetLatency(int type);
    bool isValidTick(int tick , C_BasePlayer* pLocal);
    float GetLerpTime();
    void SetValidTickCount(C_BasePlayer* player, CUserCmd* pCmd);
    void StoreData(C_BasePlayer *player);
    void RestorePlayer(C_BasePlayer* player);

};

namespace Resolver {

    extern std::vector<int64_t> playerAngleLogs;
    extern std::array<CResolveInfo, 32> m_arrInfos;

    float GetLerpTime();

    void Hug( C_BasePlayer* Circlebian );

    void FrameStageNotify( ClientFrameStage_t stage );

    void PostFrameStageNotify( ClientFrameStage_t stage );

    void FireGameEvent( IGameEvent* event );

    void CreateMove( CUserCmd* cmd );

    CTickRecord GetShotRecord( C_BasePlayer* );

    bool HasStaticRealAngle( int index, float tolerance = 15.f );

    bool HasStaticRealAngle( const std::deque<CTickRecord>& l, float tolerance = 15.f );

    bool HasStaticYawDifference( const std::deque<CTickRecord>& l, float tolerance = 15.f );

    bool HasSteadyDifference( const std::deque<CTickRecord>& l, float tolerance = 15.f );

    int GetDifferentDeltas( const std::deque<CTickRecord>& l, float tolerance = 15.f );

    int GetDifferentLBYs( const std::deque<CTickRecord>& l, float tolerance = 15.f );

    float GetLBYByComparingTicks( const std::deque<CTickRecord>& l );

    float GetDeltaByComparingTicks( const std::deque<CTickRecord>& l );

    bool DeltaKeepsChanging( const std::deque<CTickRecord>& cur, float tolerance = 15.f );

    bool LBYKeepsChanging( const std::deque<CTickRecord>& cur, float tolerance = 15.f );

    bool IsEntityMoving( C_BasePlayer* Circlebian );

    bool& LowerBodyYawChanged( C_BasePlayer* Circlebian );
}

const inline float GetDelta( float a, float b ) {
    return abs( Math::ClampYaw( a - b ) );
}

const inline float LBYDelta( const CTickRecord& v ) {
    return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
}

const inline bool IsDifferent( float a, float b, float tolerance = 10.f ) {
    return ( GetDelta( a, b ) > tolerance );
}

extern LagComp Lagcomp;