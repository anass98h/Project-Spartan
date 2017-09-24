#include "fonts.h"

HFont esp_font = 0;
HFont lby_font = 0;
char* Settings::UI::Fonts::ESP::family = XORSTR( "Sarai" );
int Settings::UI::Fonts::ESP::size = 18;
int Settings::UI::Fonts::ESP::flags = ( int ) FontFlags::FONTFLAG_OUTLINE;

void Fonts::SetupFonts() {
    esp_font = Draw::CreateFont( Settings::UI::Fonts::ESP::family, Settings::UI::Fonts::ESP::size,
                                 Settings::UI::Fonts::ESP::flags );
    lby_font = Draw::CreateFont( XORSTR( "Dyuthi" ), 28, ( int ) FontFlags::FONTFLAG_DROPSHADOW );
}
