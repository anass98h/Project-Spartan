#pragma once

#include "../Spartangui.h"

namespace HvH {

    static const char* yTypes[] = {
            "No AA", "Forwards", "Backwards", "Spin", "Fake Sideways", "Backjitter", "FJitter", "LBY Jitter", "LBY Spin",
            "LBY", "LBY Breaker", "Fake LBY", "LBY on Ground", "Tank 1", "Tank 2", "Tank 2", "Myrrib's eap",
            "Richi's eap", "Legit Trolling 1", "Legit Trolling 2", "Custom 1", "Custom 2"
    };

    static const char* lbyTypes[] = { "Breaker 1", "Breaker 2", "Breaker 3", "Avoz", "Myrrib", "None" };

    static const char* zTypes[] = { "TEST" };

    static const char* xTypes[] = { "Up", "Flip", "Down", "Fake Zer0", "Fake Up" };

    void RenderTab();
    void MainRenderTab();

}
