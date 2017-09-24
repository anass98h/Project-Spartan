#pragma once

#include "../Spartangui.h"

namespace HvH {

    static const char* yTypes[] = {
            XORSTR( "No AA" ), XORSTR( "Forwards" ), XORSTR( "Backwards" ), XORSTR( "Spin" ), XORSTR( "Fake Sideways" ),
            XORSTR( "Backjitter" ), XORSTR( "FJitter" ), XORSTR( "LBY Jitter" ), XORSTR( "LBY Spin" ), XORSTR( "LBY" ),
            XORSTR( "LBY Breaker" ), XORSTR( "Fake LBY" ), XORSTR( "LBY on Ground" ), XORSTR( "Tank 1" ),
            XORSTR( "Tank 2" ),
            XORSTR( "Tank 2" ), XORSTR( "Myrrib's eap" ), XORSTR( "Richi's eap" ), XORSTR( "Rasp 1" ),
            XORSTR( "Rasp 2" ),
            XORSTR( "FeetWiggle" ), XORSTR( "Legit Trolling 1" ), XORSTR( "Legit Trolling 2" ), XORSTR( "Custom 1" ),
            XORSTR( "Custom 2" )
    };

    static const char* lbyTypes[] = {
            XORSTR( "Breaker 1" ), XORSTR( "Breaker 2" ), XORSTR( "Breaker 3" ), XORSTR( "Avoz" ), XORSTR( "Myrrib" ),
            XORSTR( "None" )
    };

    static const char* zTypes[] = { XORSTR( "TEST" ) };

    static const char* xTypes[] = {
            XORSTR( "Up" ), XORSTR( "Flip" ), XORSTR( "Down" ), XORSTR( "Fake Zer0" ), XORSTR( "Fake Up" )
    };

    void RenderTab();
    void MainRenderTab();

}
