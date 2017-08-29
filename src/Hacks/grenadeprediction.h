#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Hooks/hooks.h"
#include "../interfaces.h"

namespace GrenadePrediction
{

    void DrawPrediction(C_BasePlayer* pLocal, C_BaseCombatWeapon* grenade);

    // Hooks
    void Paint();

}
