#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Hooks/hooks.h"
#include "../interfaces.h"
#include "aimbot.h"
#include <lua.hpp>

#define ANTIAIM_AIRBORNE 0
#define ANTIAIM_MOVING 1
#define ANTIAIM_STANDING 2

namespace AntiAim {

    extern bool isAntiAiming;
    extern float lastRealYaw;
    extern float lastFakeYaw;
    extern bool canEdge;
    extern bool fakeTp;

    void SwitchAA( int state );

    bool IsStanding();

    bool IsMoving();

    bool IsAirborne();

    //Hooks
    void CreateMove( CUserCmd* cmd );
}
