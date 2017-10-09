#pragma once

#include "../SDK/SDK.h"
#include "../settings.h"

namespace NameChanger {
    enum class Colors {
        WHITE = 1,
        DARK_RED,
        LIGHT_PURPLE,
        DARK_GREEN,
        LIGHT_GREEN,
        GREEN,
        LIGHT_RED,
        GRAY,
        YELLOW,
        LIGHT_BLUE,
        BLUE,
        DARK_BLUE,
        DARK_GRAY,
        PURPLE,
        RED,
        ORANGE,
    };
    enum class nnType {

        Spartan,
        Aimtux,
        Interwebz,
        Aimware,

    };
    enum class NC_Type {
        NC_NORMAL,
        NC_RAINBOW,
        NC_SOLID,
    };

    extern int changes;
    extern NC_Type type;
    extern nnType type2;
    extern std::string origName;
    extern Colors color;
    extern std::vector<std::pair<NameChanger::Colors, const char*>> colors;
    extern std::vector<std::pair<NameChanger::nnType, const char*>> nntypes;

    void AyywareCrash();

    void SetName( const char* name );

    void InitColorChange( NC_Type type, Colors color = Colors::LIGHT_RED );

    void InitNoName( NC_Type type, nnType type2 );

    //Hooks
    void BeginFrame( float frameTime );
};
