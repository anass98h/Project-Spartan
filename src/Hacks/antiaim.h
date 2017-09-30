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

    extern std::map<char[], float> savedAngles;

    void SaveAngle( IEngineClient::player_info_s player, float angle );

    float RestoreAngle( IEngineClient::player_info_s player );

    extern bool isAntiAiming;
    extern float lastRealYaw;
    extern float lastFakeYaw;

    void SwitchAA( int state );

    bool IsStanding();

    bool IsMoving();

    bool IsAirborne();

    //Hooks
    void CreateMove( CUserCmd* cmd );

    void FireGameEvent( IGameEvent* event );
}
