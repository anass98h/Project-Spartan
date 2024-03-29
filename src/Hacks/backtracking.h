#pragma once

#include <deque>
#include <map>
#include "../interfaces.h"
#include "../settings.h"
#include "../SDK/CTickRecord.h"

namespace Backtracking {

    const int MAX_QUEUE_SIZE = 16;

    extern std::map<int, std::deque<CTickRecord>> lagRecords; // PlayerID, TickRecord

    extern bool backtrackingLby;

    void RestorePosition( int playerIndex, int tickDiff );

    void ToggleRequiredCVars( bool activate = true );

    //Hooks

    void FrameStageNotify( ClientFrameStage_t stage );
}
