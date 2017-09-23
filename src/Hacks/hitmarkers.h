#pragma once

#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/entity.h"

namespace Hitmarkers {

    void PlayHitSound();

    //Hooks
    void FireGameEvent( IGameEvent* event );

    void Paint();
}
