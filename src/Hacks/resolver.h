#pragma once

#include "../settings.h"
#include "backtracking.h"
#include "../SDK/SDK.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/pstring.h"
#include "../Utils/entity.h"
#include "../SDK/CTickRecord.h"
#include <random>

namespace Resolver {
    extern bool lbyUpdated;

    void Hug( C_BasePlayer* pEntity );

    void FrameStageNotify( ClientFrameStage_t stage );

    void PostFrameStageNotify( ClientFrameStage_t stage );

    void CreateMove( CUserCmd* cmd );

    void ResolveY( C_BasePlayer* pEntity );
}