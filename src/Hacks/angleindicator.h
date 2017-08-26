#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Hooks/hooks.h"
#include "../interfaces.h"

namespace AngleIndicator
{

    // Hooks
    void PaintImGui();
    void PostPredictionCreateMove(CUserCmd* cmd);

}
