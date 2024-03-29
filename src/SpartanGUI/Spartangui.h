#pragma once

#include <iostream>
#include <string.h>
#include <memory>
#include <stdio.h>
#include <list>
#include <time.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"
#include "../ImGUI/imgui_impl_sdl.h"
#include "../settings.h"
#include "../Hacks/hacks.h"
#include "../Utils/skins.h"
#include "imgui.h"
#include "Windows/skinmodelchanger.h"
#include "Windows/configs.h"
#include "Windows/playerlist.h"
#include "Windows/resolverinfo.h"
#include "Windows/colors.h"
#include "Windows/login.h"
#include "Windows/main.h"
#include "Tabs/AimTabs/aimbotaimtab.h"
#include "Tabs/AimTabs/triggerbotaimtab.h"
#include "Tabs/aimtab.h"
#include "Tabs/visualstab.h"
#include "Tabs/hvhtab.h"
#include "Tabs/misctab.h"
#include "Tabs/modelstab.h"
#include "Tabs/skinstab.h"
#include "Tabs/HvHTabs/mischvhtab.h"
#include "Tabs/HvHTabs/movinghvhtab.h"
#include "Tabs/HvHTabs/standinghvhtab.h"
#include "Tabs/HvHTabs/airbornehvhtab.h"
#include "../Utils/xorstring.h"
#include "../project-spartan.h"


namespace UI {
    extern bool isVisible;

    void WeebsterMarc();

    void SetVisible( bool visible );

    void SwapWindow();

    void SetupColors();

    void SetupWindows();

    bool ColorPicker( float* col, bool alphabar );

    bool ColorPicker3( float col[3] );

    bool ColorPicker4( float col[4] );

    void KeyBindButton( ButtonCode_t* key );

    void UpdateWeaponSettings();

    void ReloadWeaponSettings();
}

inline void SetTooltip( const char* text ) {
    if ( ImGui::IsItemHovered() )
        ImGui::SetTooltip( "%s", text );
}
