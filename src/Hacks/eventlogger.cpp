#include "eventlogger.h"

bool Settings::EventLogger::damage = false;
bool Settings::EventLogger::itemPurchase = false;
bool Settings::EventLogger::beginPlant = false;
bool Settings::EventLogger::beginDefuse = false;
bool Settings::EventLogger::bombPlanted = false;
bool Settings::EventLogger::enterBombsite = false;

const char* team[] { "", "", " (T)", " (CT)" };
const char* defKit[] { "without a defuse kit.", "with a defuse kit." };

void EventLogger::PaintImGui() {
    if ( !EventLogger::IsEnabled() || !engine->IsInGame() )
        return;

    for ( size_t i = 0; i < events.size(); i++ ) {
        float time = globalVars->curtime - events[i].time;

        if ( time > 6.0f )
            events[i].color.Value.w *= 0.98f;

        if ( events[i].color.Value.w < 0.5f )
            return;

        float height = i * ImGui::GetFontSize() + 4.0f;

        Draw::ImText(
                ImVec2(4.0f, 4.0f + height), events[i].color, events[i].text.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow
        );
    }
}

void EventLogger::FireGameEvent( IGameEvent* event ) {
    if ( !event )
        return;

    if ( strstr( event->GetName(), XORSTR( "player_hurt" )) && Settings::EventLogger::damage ) {
        C_BasePlayer* hurt = (C_BasePlayer*) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" )))
        );
        C_BasePlayer* attacker  = (C_BasePlayer*) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "attacker" )))
        );
        int damage = event->GetInt( XORSTR( "dmg_health" ));

        IEngineClient::player_info_t hurtPlayerInfo;
        engine->GetPlayerInfo( hurt->GetIndex(), &hurtPlayerInfo );

        IEngineClient::player_info_t attackerInfo;
        engine->GetPlayerInfo( attacker->GetIndex(), &attackerInfo );

        std::stringstream text;
        text << attackerInfo.name << XORSTR( " hit " ) << hurtPlayerInfo.name << XORSTR( " for " ) << damage << XORSTR( " HP" );

        if ( hurt->GetAlive() &&
                attacker->GetIndex() == entityList->GetClientEntity(engine->GetLocalPlayer())->GetIndex() ) {
            int health = hurt->GetHealth() - damage;

            if ( health > 0 ) {
                text << XORSTR( " (" ) << health << XORSTR( " HP remaining)" );
            }
        }

        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "item_purchase" )) && Settings::EventLogger::itemPurchase ) {
        C_BasePlayer* user = (C_BasePlayer*) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" )))
        );
        const char* weapon = event->GetString( XORSTR( "weapon" ));

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << team[event->GetInt( XORSTR( "team" ))] << XORSTR( " purchased " ) << weapon;
        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "bomb_beginplant" )) && Settings::EventLogger::beginPlant ) {
        C_BasePlayer* user = (C_BasePlayer*) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" )))
        );
        int site = event->GetInt( XORSTR( "site" ));

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " started planting at bombsite " ) << site;
        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "bomb_begindefuse" )) && Settings::EventLogger::beginDefuse ) {
        C_BasePlayer* user = (C_BasePlayer*) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" )))
        );
        bool defuser = event->GetBool( XORSTR( "haskit" ));

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        std::stringstream text;
        text << entityInfo.name << XORSTR( " started defusing the bomb " ) << defKit[defuser];
        EventLogger::AddEvent( text.str() );
    }

    if ( strstr( event->GetName(), XORSTR( "bomb_planted" )) && Settings::EventLogger::bombPlanted ) {
        int site = event->GetInt( XORSTR( "site" ));

        std::stringstream text( XORSTR( "The bomb has been planted at bombsite " ));
        text << site;
        EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
    }

    if ( strstr( event->GetName(), XORSTR(" enter_bombzone" )) && Settings::EventLogger::enterBombsite ) {
        C_BasePlayer* user = (C_BasePlayer*) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" )))
        );
        bool bomb = event->GetBool( XORSTR( "hasbomb" ));

        IEngineClient::player_info_t entityInfo;
        engine->GetPlayerInfo( user->GetIndex(), &entityInfo );

        if ( !event->GetBool( XORSTR( "isplanted" )) && bomb ) {
            std::stringstream text( XORSTR( "Bomb carrier " ));
            text << entityInfo.name << " has entered a bombsite";
            EventLogger::AddEvent( text.str(), ImColor( 26, 104, 173 ) );
        }
    }

    if (events.size() > 12) {
        events.pop_back();
    }
}

void EventLogger::AddEvent(std::string text, ImColor color) {
    events.push_front(loginfo(text, color, globalVars->curtime));

    if ( events.size() > 12 ) {
        events.pop_back();
    }
}

bool EventLogger::IsEnabled() {
    return Settings::EventLogger::damage ||
            Settings::EventLogger::itemPurchase ||
            Settings::EventLogger::beginPlant ||
            Settings::EventLogger::beginDefuse ||
            Settings::EventLogger::bombPlanted ||
            Settings::EventLogger::enterBombsite;
}
