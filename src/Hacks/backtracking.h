#pragma once

#include <deque>
#include <algorithm>
#include <map>
#include "../interfaces.h"
#include "../settings.h"
#include "../SDK/CTickRecord.h"

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / globalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )         ( globalVars->interval_per_tick * ( t ) )

namespace Backtracking {

    const int MAX_QUEUE_SIZE = 16;

    extern std::map<int, std::deque<CTickRecord>> lagRecords; // PlayerID, TickRecord

    void RestorePosition( int playerIndex, int tickDiff );

    void ToggleRequiredCVars( bool activate = true );

    //Hooks

    void FrameStageNotify( ClientFrameStage_t stage );
}

struct StoredNetvars {
    float lby;
    Vector origin;
    Vector min;
    Vector max;
    int flags;
    float simulationtime;
    QAngle eyeangles;
    std::array<float, 24> poseparam;
    int sequence;
    float cycle;

    Vector velo;

    bool lbyupdate;
};

class CLagCompensation2 {
private:
    void SaveNetvars(StoredNetvars *dest, C_BasePlayer *player);
    void RestoreNetvars(StoredNetvars *src, C_BasePlayer *player);
public:
    std::vector<StoredNetvars> vecLagRecord[64];
    StoredNetvars *pRecordRollback[64];

    float GetLatency(int type);
    bool isValidTick(int tick);
    float GetLerpTime();
    void SetValidTickCount(C_BasePlayer *player, CUserCmd* pCmd);
    void StoreDatas(C_BasePlayer *player);
    int GetLastLbyRecord(C_BasePlayer *player);
    void RestorePlayer(C_BasePlayer *player);
    bool isLbyUpdate(C_BasePlayer *player);
    static void Store(C_BasePlayer *player);
    static void Restore(C_BasePlayer *player);
};

extern CLagCompensation2 LagCompensation2;