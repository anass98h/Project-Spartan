#include "clantagchanger.h"

char Settings::ClanTagChanger::value[30] = "";
bool Settings::ClanTagChanger::animation = false;
int Settings::ClanTagChanger::animationSpeed = 300;
bool Settings::ClanTagChanger::enabled = false;
ClanTagType Settings::ClanTagChanger::type = ClanTagType::STATIC;
valueType Settings::ClanTagChanger::preset = valueType::PSPARTAN;
static int clanID = -1;

void ClanTagChanger::SaveOriginalClanTag() {
    if(clanID == -1) {
        ConVar* clClanID = cvar->FindVar(XORSTR("cl_clanid"));

        clanID = clClanID->GetInt();
        clClanID->SetValue(0);
    }
}

void ClanTagChanger::RestoreOriginalClanTag() {
    if ( clanID != -1 ) {
        ConVar* clClanID = cvar->FindVar( XORSTR( "cl_clanid" ) );

        if ( clanID == 0 ) {
            // Workaround in case the player didn't set a clantag (so we set it back to 0)
            clClanID->SetValue( 1 );
        }
        clClanID->SetValue( clanID );
        clanID = -1;
    }
}

static std::vector<std::wstring> splitWords( std::wstring text ) {
    std::wistringstream stream( text );
    std::wstring word;
    std::vector<std::wstring> words;
    while ( stream >> word )
        words.push_back( word );

    return words;
}

static const char* PresetChars() {

    switch ( Settings::ClanTagChanger::preset ) {
        case valueType::PSPARTAN:
            return "Project-Spartan";

            break;
        case valueType::NOVAC:
            return "cantvac.me";

            break;
        case valueType::TUXCH:
            return "tuxcheats.com";

            break;
        case valueType::REALNIGGA:
            return "realnigga.club";

            break;
        case valueType::MARC:
            return "void MarcIsAWeeb()";

            break;
        case valueType::CUSTOM:
            return Settings::ClanTagChanger::value;

            break;
    }


}

static ClanTagChanger::Animation Letters( std::string name, std::wstring text ) {
    // Outputs a letter incrementing animation
    std::vector<ClanTagChanger::Frame> frames;
    for ( unsigned long i = 1; i <= text.length(); i++ )
        frames.push_back( ClanTagChanger::Frame( text.substr( 0, i ), Settings::ClanTagChanger::animationSpeed ) );

    for ( unsigned long i = text.length() - 2; i > 0; i-- )
        frames.push_back( ClanTagChanger::Frame( frames[i].text, Settings::ClanTagChanger::animationSpeed ) );

    return ClanTagChanger::Animation( name, frames, ClanTagChanger::ANIM_LOOP );
}

static ClanTagChanger::Animation Marquee( std::string name, std::wstring text, int width = 15 ) {
    text.erase( std::remove( text.begin(), text.end(), '\0' ), text.end() );

    std::wstring cropString = std::wstring( width, ' ' ) + text + std::wstring( width - 1, ' ' );

    std::vector<ClanTagChanger::Frame> frames;
    for ( unsigned long i = 0; i < text.length() + width; i++ )
        frames.push_back(
                ClanTagChanger::Frame( cropString.substr( i, width + i ), Settings::ClanTagChanger::animationSpeed ) );

    return ClanTagChanger::Animation( name, frames, ClanTagChanger::ANIM_LOOP );
}

static ClanTagChanger::Animation Words( std::string name, std::wstring text ) {
    // Outputs a word by word animation
    std::vector<std::wstring> words = splitWords( text );
    std::vector<ClanTagChanger::Frame> frames;
    for ( unsigned long i = 0; i < words.size(); i++ )
        frames.push_back( ClanTagChanger::Frame( words[i], Settings::ClanTagChanger::animationSpeed ) );

    return ClanTagChanger::Animation( name, frames, ClanTagChanger::ANIM_LOOP );
}

std::vector<ClanTagChanger::Animation> ClanTagChanger::animations = {

        Marquee( "--", L"--" ),
        Words( "--", L"--" ),
        Letters( "--", L"--" )
};
ClanTagChanger::Animation* ClanTagChanger::animation = &ClanTagChanger::animations[0];

void ClanTagChanger::UpdateClanTagCallback() {
    if ( Settings::ClanTagChanger::type > ClanTagType::STATIC ) {
        std::wstring wc = Util::StringToWstring( std::string( PresetChars() ) );

        switch ( Settings::ClanTagChanger::type ) {

            case ClanTagType::MARQUEE:
                *ClanTagChanger::animation = Marquee( "------", wc );
                break;
            case ClanTagType::WORDS:
                *ClanTagChanger::animation = Words( "------", wc );
                break;
            case ClanTagType::LETTERS:
                *ClanTagChanger::animation = Letters( "------", wc );
                break;
            default:
                break;
        }

        return;
    }

    ClanTagChanger::animations = {
            Marquee( "--", L"--" ),
            Words( "--", L"--" ),
            Letters( "--", L"--" )
    };

    int current_animation = ( int ) Settings::ClanTagChanger::type - 1;
    if ( current_animation >= 0 )
        ClanTagChanger::animation = &ClanTagChanger::animations[current_animation];
}

void ClanTagChanger::BeginFrame( float frameTime ) {
    if ( !Settings::ClanTagChanger::enabled ) {
        ClanTagChanger::RestoreOriginalClanTag();
        return;
    } else {
        ClanTagChanger::SaveOriginalClanTag();
    }

    if ( !engine->IsInGame() )
        return;

    long currentTime_ms = Util::GetEpochTime();
    static long timeStamp = currentTime_ms;

    if ( currentTime_ms - timeStamp > ClanTagChanger::animation->GetCurrentFrame().time ) {
        timeStamp = currentTime_ms;
        ClanTagChanger::animation->NextFrame();
    }


    if ( Settings::ClanTagChanger::type == ClanTagType::CURTIME ) {
        time_t rawtime;
        struct tm* timeinfo;
        char buffer[80];

        time( &rawtime );
        timeinfo = localtime( &rawtime );

        strftime( buffer, sizeof( buffer ), "%T %Z", timeinfo );
        std::string str( buffer );

        SendClanTag( str.c_str(), "" );
        return;
    } else {
        std::string ctWithEscapesProcessed = std::string( PresetChars() );
        Util::StdReplaceStr( ctWithEscapesProcessed, "\\n",
                             "\n" );


        if ( Settings::ClanTagChanger::type == ClanTagType::STATIC )
            SendClanTag( ctWithEscapesProcessed.c_str(), "" );
        else
            SendClanTag( Util::WstringToString( ClanTagChanger::animation->GetCurrentFrame().text ).c_str(), "" );
    }
}
