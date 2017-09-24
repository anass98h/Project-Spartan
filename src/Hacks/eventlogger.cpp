#include "eventlogger.h"

bool Settings::EventLogger::enabled = false;

const char* team[]{ XORSTR( "" ), XORSTR( "" ), XORSTR( " (T)" ), XORSTR( " (CT)" ) };
const char* defKit[]{ XORSTR( "without a defuse kit." ), XORSTR( "with a defuse kit." ) };
std::map<int, const char*> bombsites = {
        { 369, XORSTR( "Bombsite A" ) }, // Dust 2
        { 366, XORSTR( "Bombsite B" ) }, // Dust 2
        { 451, XORSTR( "Bombsite A" ) }, // Mirage
        { 452, XORSTR( "Bombsite B" ) }, // Mirage
        { 315, XORSTR( "Bombsite A" ) }, // Cache
        { 316, XORSTR( "Bombsite B" ) }, // Cache
        { 334, XORSTR( "Bombsite A" ) }, // Inferno
        { 423, XORSTR( "Bombsite B" ) }, // Inferno
        { 260, XORSTR( "Bombsite A" ) }, // Cobblestone
        { 95,  XORSTR( "Bombsite B" ) }, // Cobblestone
        { 79,  XORSTR( "Bombsite A" ) }, // Overpass
        { 507, XORSTR( "Bombsite B" ) }, // Overpass
        { 149, XORSTR( "Bombsite A" ) }, // Nuke
        { 441, XORSTR( "Bombsite B" ) }, // Nuke
        { 93,  XORSTR( "Bombsite A" ) }, // Train
        { 538, XORSTR( "Bombsite B" ) }, // Train
        { 222, XORSTR( "Bombsite A" ) }, // Canals
        { 221, XORSTR( "Bombsite B" ) }, // Canals
        { 239, XORSTR( "Bombsite A" ) }, // Aztec
        { 238, XORSTR( "Bombsite B" ) }, // Aztec
        { 81,  XORSTR( "Bombsite A" ) }, // Vertigo
        { 82,  XORSTR( "Bombsite B" ) }, // Vertigo
        { 526, XORSTR( "Bombsite A" ) }, // Dust 1
        { 525, XORSTR( "Bombsite B" ) }, // Dust 1
};

void Capitalize( std::string& s );

std::string GetBombsiteByID( int id );

void EventLogger::PaintImGui() {
    if ( !Settings::EventLogger::enabled || !engine->IsInGame() )
        return;

    for ( size_t i = 0; i < events.size(); i++ ) {
        float time = globalVars->curtime - events[i].time;

        if ( time > 6.0f )
            events[i].color.Value.w *= 0.98f;

        if ( events[i].color.Value.w < 0.5f )
            return;

        float height = i * ImGui::GetFontSize() + 4.0f;

        Draw::ImText(
                ImVec2( 4.0f, 4.0f + height ), events[i].color, events[i].text.c_str(), NULL, 0.0f, NULL,
                ImFontFlags_Shadow
        );
    }
}

void EventLogger::FireGameEvent( IGameEvent* event ) {
    if ( !event )
        return;

    if ( !Settings::EventLogger::enabled )
        return;

    if ( strstr( event->GetName(), XORSTR( "player_hurt" ) ) ) {
        C_BasePlayer* hurt = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );
        C_BasePlayer* attacker = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "attacker" ) ) )
        );
        int damage = event->GetInt( XORSTR( "dmg_health" ) );

        IEngineClient::player_info_t hurtPlayerInfo;
        engine->GetPlayerInfo( hurt->GetIndex(), &hurtPlayerInfo );

        IEngineClient::player_info_t attackerInfo;
        engine->GetPlayerInfo( attacker->GetIndex(), &attackerInfo );

        std::string attackerName = attackerInfo.name;
        if ( event->GetInt( XORSTR( "attacker" ) ) == 0 ) {
            attackerName = XORSTR( "World" );
        }

        std::stringstream text;
        text << attackerName << XORSTR( " hit " ) << hurtPlayerInfo.name << XORSTR( " for " ) << damage
             << XORSTR( " HP" );

        if ( hurt->GetAlive() ) {
            if ( !hurt->GetDormant() ) {
                int health = hurt->GetHealth() - damage;

                if ( health > 0 ) {
                    text << XORSTR( " (" ) << health << XORSTR( " HP remaining)" );
                } else {
                    text << XORSTR( " (Dead)" );
                }
            }
        } else {
            text << XORSTR( " (Dead)" );
        }

        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "item_purchase" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );
        std::string weapon = std::string( event->GetString( XORSTR( "weapon" ) ) );
        int teamNum = event->GetInt( XORSTR( "team" ) );

        if ( !user )
            return;

        if ( weapon.empty() )
            return;

        if ( !teamNum )
            return;

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        if ( !entityInfo.name )
            return;

        std::string prefix( XORSTR( "weapon_" ) );
        if ( strncmp( weapon.c_str(), prefix.c_str(), prefix.size() ) == 0 ) {
            weapon = weapon.substr( prefix.size() );
        }
        std::replace( weapon.begin(), weapon.end(), '_', ' ' );
        Capitalize( weapon );

        std::stringstream text;
        text << entityInfo.name << team[teamNum] << XORSTR( " purchased " ) << weapon;
        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "player_given_c4" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " is now the bomb carrier" );
        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "bomb_beginplant" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );
        int site = event->GetInt( XORSTR( "site" ) );

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " started planting at " ) << GetBombsiteByID( site );
        EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
    }

    if ( strstr( event->GetName(), XORSTR( "bomb_begindefuse" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );
        bool defuser = event->GetBool( XORSTR( "haskit" ) );

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " started defusing the bomb " ) << defKit[defuser];
        EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
    }

    if ( strstr( event->GetName(), XORSTR( "hostage_follows" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " has picked up a hostage" );
        EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
    }


    if ( strstr( event->GetName(), XORSTR( "hostage_rescued" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " has rescued a hostage" );
        EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
    }

    if ( strstr( event->GetName(), XORSTR( "bomb_planted" ) ) ) {
        int site = event->GetInt( XORSTR( "site" ) );

        std::stringstream text( XORSTR( "" ) );
        text << XORSTR( "The bomb has been planted at " ) << GetBombsiteByID( site );
        EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
    }

    if ( strstr( event->GetName(), XORSTR( "enter_bombzone" ) ) ) {
        C_BasePlayer* user = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );
        bool bomb = event->GetBool( XORSTR( "hasbomb" ) );

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        if ( !event->GetBool( XORSTR( "isplanted" ) ) && bomb ) {
            std::stringstream text( XORSTR( "Bomb carrier " ) );
            text << entityInfo.name << " has entered a bombsite";
            EventLogger::AddEvent( text.str() );
        }
    }

    if ( events.size() > 12 ) {
        events.pop_back();
    }
}

void EventLogger::AddEvent( std::string text, ImColor color ) {
    events.push_front( loginfo( text, color, globalVars->curtime ) );

    if ( events.size() > 12 ) {
        events.pop_back();
    }
}

std::string GetBombsiteByID( int id ) {
    if ( bombsites.find( id ) == bombsites.end() ) {
        cvar->ConsoleColorPrintf( ColorRGBA( 255, 255, 255 ), XORSTR( "(EventLogger::GetBombsiteByID): Warning: "
                                                                              "Could not find localized string for ID \"%i\".\n" ),
                                  id );

        return XORSTR( "bombsite" );
    }

    return bombsites.at( id );
}

void Capitalize( std::string& s ) {
    bool cap = true;

    for ( unsigned int i = 0; i <= s.length(); i++ ) {
        if ( isalpha( s[i] ) && cap ) {
            s[i] = toupper( s[i] );
            cap = false;
        } else if ( isspace( s[i] ) ) {
            cap = true;
        }
    }
}
