#pragma once

#include <deque>
#include <map>
#include "../interfaces.h"
#include "../settings.h"
#include "../SDK/CTickRecord.h"


namespace Backtracking {
    const int MAX_QUEUE_SIZE = 16;
    extern std::map<int, std::deque<CTickRecord>> lagRecords; // PlayerID, TickRecord

    void RestorePosition( int playerIndex, int tickDiff );

    //Hooks
    void FrameStageNotify( ClientFrameStage_t stage );
}

