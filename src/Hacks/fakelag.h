#pragma once

#include "../settings.h"
#include "../interfaces.h"
#include "../Hooks/hooks.h"
#include "../Utils/util.h"

namespace FakeLag {
    extern bool pSilent;
    extern bool bFlipping;

    //Hooks
    void CreateMove( CUserCmd *cmd );
};
