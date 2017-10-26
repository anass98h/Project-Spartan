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
#define ACT_CSGO_IDLE_TURN_BALANCEADJUST 979

enum Angle {
    LBY = 0,
    LBY180,
    LBYP90,
    LBYM90
};

namespace Resolver {

    void Hug( C_BasePlayer* target );

    float HugLby( C_BasePlayer* target );

    float HugBrute( C_BasePlayer* target );

    float HugPitch( C_BasePlayer* target );

    // Variables

    extern bool baimNextShot;

    extern bool lbyUpdated;

    extern int resolvingId;

    extern std::map<int, float> lby;

    extern std::map<int, bool> hasFakeWalk;

    extern std::map<int, float> lastHitAng;

    extern std::map<int, const char*> lastHitAngTxt;    

    extern std::map<int, float> angForce;

    extern std::map<int, const char*> angForceTxt;

    extern std::map<int, int> shotsMissedSave;

    extern std::map<int, Angle> lastForceAng;

    // Hooks

    void CreateMove( CUserCmd* cmd );

    void FrameStageNotify( ClientFrameStage_t stage );

    void PostFrameStageNotify( ClientFrameStage_t stage );

    void FireGameEvent( IGameEvent* event );

    void Paint();

}
