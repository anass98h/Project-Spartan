#pragma once

#include "interfaces.h"
#include "Hacks/hacks.h"
#include "shortcuts.h"
#include <string.h>

#include "settings.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/fonts/KaiGenGothicCNRegular.h"
//#include "ImGUI/fonts/SegoeUI.h"
#include "ImGUI/IconsFontAmazing/IconsFontAwesome.h"
#include "ImGUI/fonts/Tux.h"
#include "ImGUI/fonts/Tux2.h"
#include "SpartanGUI/arial.h"


typedef void (* SDL_GL_SwapWindow_t)( SDL_Window* );

typedef int (* SDL_PollEvent_t)( SDL_Event* );


namespace SDL2 {
    void SwapWindow( SDL_Window* );

    void UnhookWindow();

    int PollEvent( SDL_Event* );

    void UnhookPollEvent();

    //Menu Icons
#define ICON_1 u8"\uFFDE"
#define ICON_2 u8"\uFFDF"
#define ICON_3 u8"\uFFE0"
#define ICON_4 u8"\uFFE1"
#define ICON_5 u8"\uFFE2"
#define ICON_6 u8"\uFFE3"

#define ICON_7 u8"\uFFE5"
#define ICON_8 u8"\uFFE6"
#define ICON_9 u8"\uFFE7"
#define ICON_10 u8"\uFFE8"
#define ICON_11 u8"\uFFE9"
#define ICON_12 u8"\uFFEA"

#define ICON_MIN 0xe000
#define ICON_MAX 0xe066

    //Weapon icons
#define ICON_BEANER u8"\ue000"
#define ICON_DEAG u8"\ue001"
#define ICON_DUALIES u8"\ue002"
#define ICON_FIVESEVEN u8"\ue003"
#define ICON_GLOCK u8"\ue004"
#define ICON_AK47 u8"\ue007"
#define ICON_AUG u8"\ue008"
#define ICON_AWP u8"\ue009"
#define ICON_FAMAS u8"\ue00A"
#define ICON_G3SG1 u8"\ue00B"
#define ICON_GALIL u8"\ue00D"
#define ICON_M4A4 u8"\ue00E"
#define ICON_M4A1S u8"\ue010"
#define ICON_MAC10 u8"\ue011"
#define ICON_P200 u8"\ue013"
#define ICON_UMP u8"\ue018"
#define ICON_XM1014 u8"\ue019"
#define ICON_PPBIZON u8"\ue01A"
#define ICON_MAG7 u8"\ue01B"
#define ICON_NEGEV u8"\ue01C"
#define ICON_SAWNOFF u8"\ue01D"
#define ICON_TEC9 u8"\ue01E"
#define ICON_ZEUS u8"\ue01F"
#define ICON_P250 u8"\ue020"
#define ICON_MP7 u8"\ue021"
#define ICON_MP9 u8"\ue022"
#define ICON_NOVA u8"\ue023"
#define ICON_P90 u8"\ue024"
#define ICON_SCAR20 u8"\ue026"
#define ICON_SG553 u8"\ue027"
#define ICON_SSG08 u8"\ue028"
#define ICON_CTKNIFE u8"\ue02A"
#define ICON_FLASH u8"\ue02B"
#define ICON_FRAG u8"\ue02C"
#define ICON_SMOKE u8"\ue02D"
#define ICON_MOLOTOV u8"\ue02E"
#define ICON_DECOY u8"\ue02F"
#define ICON_INCENDIARY u8"\ue030"
#define ICON_BOMB u8"\ue031"
#define ICON_TKNIFE u8"\ue03B"
#define ICON_M249 u8"\ue03C"
#define ICON_USPS u8"\ue03D"
#define ICON_CZ75 u8"\ue03F"
#define ICON_REVOLVER u8"\ue040"
#define ICON_KELVAR u8"\ue064"
#define ICON_KELVARHLMET u8"\ue065"
#define ICON_DEFUSER u8"\ue066"
    //Other icons
#define ICON_SPECTATORS u8"\ue04B"
#define ICON_SKINCHANGER u8"\ue04D"
#define ICON_PLAYERLIST u8"\ue04E"
#define ICON_RAGE u8"\ue04F"
#define ICON_CONFIGS u8"\ue050"
#define ICON_COLORS u8"\ue051"
#define ICON_SETTINGS u8"\ue052"
#define ICON_COLOURS u8"\ue053"
}
