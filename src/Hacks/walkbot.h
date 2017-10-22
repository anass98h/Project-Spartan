#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../interfaces.h"
#include <vector>

namespace WalkBot {

    struct WalkBotPointInfo {

        std::vector<Vector> tRallyPoints = {};
        std::vector<Vector> ctRallyPoints = {};

    };

    extern std::map<std::string, WalkBot::WalkBotPointInfo> map;

    extern bool shouldForceReset;

    extern void ForceReset();

    // Hooks

    void CreateMove( CUserCmd* cmd );

}
