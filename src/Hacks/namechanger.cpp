#include "namechanger.h"

std::string NameChanger::origName = "";
int NameChanger::changes = -1;
NameChanger::NC_Type NameChanger::type = NC_Type::NC_NORMAL;
NameChanger::nnType NameChanger::type2 = nnType::Spartan;
NameChanger::Colors NameChanger::color = Colors::LIGHT_RED;
std::vector<std::pair<NameChanger::Colors, const char*>> NameChanger::colors = {
        { NameChanger::Colors::WHITE,        "White" },
        { NameChanger::Colors::DARK_RED,     "Dark Red" },
        { NameChanger::Colors::LIGHT_PURPLE, "Light Purple" },
        { NameChanger::Colors::DARK_GREEN,   "Dark Green" },
        { NameChanger::Colors::LIGHT_GREEN,  "Light Green" },
        { NameChanger::Colors::GREEN,        "Green" },
        { NameChanger::Colors::LIGHT_RED,    "Light Red" },
        { NameChanger::Colors::GRAY,         "Gray" },
        { NameChanger::Colors::YELLOW,       "Yellow" },
        { NameChanger::Colors::LIGHT_BLUE,   "Light Blue" },
        { NameChanger::Colors::BLUE,         "Blue" },
        { NameChanger::Colors::DARK_BLUE,    "Dark Blue" },
        { NameChanger::Colors::DARK_GRAY,    "Dark Gray" },
        { NameChanger::Colors::PURPLE,       "Purple" },
        { NameChanger::Colors::RED,          "Red" },
        { NameChanger::Colors::ORANGE,       "Orange" },
};
std::vector<std::pair<NameChanger::nnType, const char*>> NameChanger::nntypes = {
        { NameChanger::nnType::Spartan,   "Spartan" },
        { NameChanger::nnType::Aimtux,    "Aimtux" },
        { NameChanger::nnType::Interwebz, "Interwebz" },
        { NameChanger::nnType::Aimware,   "Aimware" },
};
bool switcher = false;

static std::string GetName() {
    IEngineClient::player_info_t playerInfo;
    engine->GetPlayerInfo( engine->GetLocalPlayer(), &playerInfo );
    return std::string( playerInfo.name );
}

void NameChanger::AyywareCrash() {
    /*for ( int i = 0; i < 6; i++ ) {
        static bool ayywareS = false;
        ayywareS = !ayywareS;
        if ( ayywareS ) {
            SetName(
                    "-AyyWare Crasher 2k17 --- Project Spartan --- CrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWare" );
        } else {
            SetName(
                    "AyyWare Crasher 2k17 --- Project Spartan --- CrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWareCrashAyyWare" );
        }
    }*/

    std::stringstream ss;

    for ( auto i = 0; i <= 150; i++ ) {
        ss << static_cast<char>(-1);
        //Breaks it and doesnt work thanks marc
        //const char char_max = (char)(((unsigned char) char(-1)) / 2);
        //char c = (-1 & char_max); // Results in not overflowing
    }

    SetName( ss.str().c_str() );
}

static std::string Colorize( const std::string& name, NameChanger::Colors color = NameChanger::Colors::LIGHT_RED ) {
    std::string res = " \x01\x0B";
    res += ( char ) ( color );
    res.append( name );
    res.append( "\230" );
    return res;
}

void NameChanger::SetName( const char* name ) {
    ConVar* cvar_name = cvar->FindVar( XORSTR( "name" ) );
    *( int* ) ( ( uintptr_t ) &cvar_name->fnChangeCallback + 0x15 ) = 0;
    cvar_name->SetValue( name );
}

void NameChanger::InitColorChange( NameChanger::NC_Type type,
                                   NameChanger::Colors color /*= NameChanger::Colors::LIGHT_RED*/) {
    NameChanger::changes = 0;
    NameChanger::origName = GetName();
    NameChanger::type = type;
    NameChanger::color = color;
}

void NameChanger::InitNoName( NameChanger::NC_Type type, NameChanger::nnType type2 ) {

    NameChanger::changes = 0;
    NameChanger::origName = GetName();
    NameChanger::type = type;
    NameChanger::type2 = type2;

}

void NameChanger::BeginFrame( float frameTime ) {
    if ( !engine->IsInGame() )
        return;

    if ( changes == -1 )
        return;

    long currentTime_ms = Util::GetEpochTime();
    static long timeStamp = currentTime_ms;

    if ( currentTime_ms - timeStamp < 150 )
        return;

    timeStamp = currentTime_ms;
    changes++;

    static int lelezspam = 6;

    if ( NameChanger::type == NC_Type::NC_RAINBOW ) {
        lelezspam = 1;
    }

    if ( lelezspam < 100 ) {
        AyywareCrash();
        lelezspam++;
    }

    if ( changes >= 5 ) {
        switch ( NameChanger::type ) {
            case NC_Type::NC_NORMAL:
                switch ( NameChanger::type2 ) {

                    case nnType::Spartan:
                        if ( !switcher ) {

                            SetName(Util::PadStringRight( XORSTR( "\230Project Spartan" ), strlen( XORSTR( "Project Spartan" ) ) + Util::RandomInt( 10, 50 ) ) );
                            switcher = !switcher;
                        } else {

                            SetName( Util::PadStringRight( XORSTR( "\230cantvac.me" ),
                                                           strlen( XORSTR( "\230cantvac.me" ) ) +
                                                           Util::RandomInt( 10, 50 ) ) );
                            switcher = !switcher;
                        }

                        break;
                    case nnType::Aimtux:
                        SetName( Util::PadStringRight( XORSTR( "\230AIMTUX.NET" ),
                                                       strlen( XORSTR( "\230AIMTUX.NET" ) ) +
                                                       Util::RandomInt( 10, 50 ) ) );
                        break;
                    case nnType::Interwebz:
                        if ( !switcher ) {

                            SetName( XORSTR( "-INTERWEBZ" ) );
                            switcher = !switcher;
                        } else {

                            SetName(  XORSTR( "INTERWEBZ-" ));
                            switcher = !switcher;
                        }

                        break;
                    case nnType::Aimware:
                        if ( !switcher ) {

                            SetName( XORSTR( "\xe1\xab\xa8 AIMWARE.net" ));
                            switcher = !switcher;
                        } else {

                            SetName(  XORSTR( "\xe1\xab\xa5 AIMWARE.net" ));
                            switcher = !switcher;
                        }

                        break;

                }

                break;
            case NC_Type::NC_SOLID:
                SetName( Util::PadStringRight( Colorize( origName, NameChanger::color ),
                                               origName.size() + Util::RandomInt( 10, 50 ) ) );
                break;
        }

        changes = -1;

        return;
    }
    if ( NameChanger::type != NC_Type::NC_RAINBOW ){
        switch ( NameChanger::type2 ) {

            case nnType::Spartan:
                if ( !switcher ) {

                    SetName( Util::PadStringRight( XORSTR( "Project Spartan" ),
                                                   strlen( XORSTR( "Project Spartan" ) ) +
                                                           changes ) );
                    switcher = !switcher;
                } else {

                    SetName( Util::PadStringRight( XORSTR( "cantvac.me" ),
                                                   strlen( XORSTR( "cantvac.me" ) ) +
                                                           changes ) );
                    switcher = !switcher;
                }

                break;
            case nnType::Aimtux:
                SetName( Util::PadStringRight( XORSTR( "AIMTUX.NET" ),
                                               strlen( XORSTR( "AIMTUX.NET" ) ) +
                                                       changes ) );
                break;
            case nnType::Interwebz:
                if ( !switcher ) {

                    SetName(  XORSTR( "-INTERWEBZ" ) );
                    switcher = !switcher;
                } else {

                    SetName(  XORSTR( "INTERWEBZ-" ) );
                    switcher = !switcher;
                }

                break;
            case nnType::Aimware:
                if ( !switcher ) {

                    SetName(  XORSTR( "\xe1\xab\xa8 AIMWARE.net" ) );
                    switcher = !switcher;
                } else {

                    SetName(  XORSTR( "\xe1\xab\xa5 AIMWARE.net" ) );
                    switcher = !switcher;
                }

                break;


        }



    }
}