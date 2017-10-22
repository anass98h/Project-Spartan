#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/pstring.h"
#include "../Utils/entity.h"
#include "../SDK/CTickRecord.h"
#include "backtracking.h"
#include <random>

#define TIME_TO_TICKS( dt ) ( (int)( 0.5f + (float)(dt) / globalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )  ( globalVars->interval_per_tick * ( t ) )

namespace Resolver {

    void Hug( C_BasePlayer* target );

    float HugLby( C_BasePlayer* target );

    float HugBrute( C_BasePlayer* target );

    void HugPitch( C_BasePlayer* target );

    // Variables

    extern bool lbyUpdated;
    extern std::map<int, int> shotsMissed;
    extern std::map<int, int> shotsMissedSave;
    extern int resolvingId;
    extern bool baimNextShot;

    // Hooks

    void CreateMove( CUserCmd* cmd );

    void FrameStageNotify( ClientFrameStage_t stage );

    void PostFrameStageNotify( ClientFrameStage_t stage );

    void Paint();

}
