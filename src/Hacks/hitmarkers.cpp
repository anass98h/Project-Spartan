#include "hitmarkers.h"

bool Settings::ESP::Hitmarker::enabled = false;
bool Settings::ESP::Hitmarker::enemies = false;
bool Settings::ESP::Hitmarker::allies = false;
ColorVar Settings::ESP::Hitmarker::color = ImColor( 255, 0, 0, 255 );
int Settings::ESP::Hitmarker::duration = 2000;
int Settings::ESP::Hitmarker::size = 16;
int Settings::ESP::Hitmarker::innerGap = 5;
bool Settings::ESP::Hitmarker::Damage::enabled = false;
bool Settings::ESP::Hitmarker::Sounds::enabled = false;
Sound Settings::ESP::Hitmarker::Sounds::sound = Sound::NONE;
// int - damage dealt, long - timestamp
std::vector<std::pair<int, long>> damages;
long lastHitmarkerTimestamp = 0;

void Hitmarkers::Paint() {
    if ( !Settings::ESP::enabled || !Settings::ESP::Hitmarker::enabled )
        return;

    if ( !engine->IsInGame() )
        return;

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer )
        return;

    if ( !localplayer->GetAlive() )
        return;

    int duration = Settings::ESP::Hitmarker::duration;
    long now = Util::GetEpochTime();

    long diff = lastHitmarkerTimestamp + duration - now;
    if ( diff <= 0 )
        return;

    int width, height;
    engine->GetScreenSize( width, height );

    Color color = Color::FromImColor( Settings::ESP::Hitmarker::color.Color() );
    color.a = std::min( color.a, ( int ) ( diff * color.a / duration * 2 ) );

    int sides[4][2] = {
            { -1, -1 },
            { 1,  1 },
            { -1, 1 },
            { 1,  -1 }
    };
    for ( auto& it : sides )
        Draw::Line( width / 2 + ( Settings::ESP::Hitmarker::innerGap * it[0] ),
                    height / 2 + ( Settings::ESP::Hitmarker::innerGap * it[1] ),
                    width / 2 + ( Settings::ESP::Hitmarker::size * it[0] ),
                    height / 2 + ( Settings::ESP::Hitmarker::size * it[1] ), color );

    if ( !Settings::ESP::Hitmarker::Damage::enabled )
        return;

    float textHeight = Draw::GetTextSize( XORSTR( "[cool]" ), esp_font ).y;

    for ( unsigned int i = 0; i < damages.size(); i++ ) {
        long timestamp = damages[i].second;
        long hitDiff = timestamp + duration - now;

        if ( hitDiff <= 0 ) {
            damages.erase( damages.begin() + i );
            continue;
        }

        Vector2D pos = Vector2D(
                width / 2 + Settings::ESP::Hitmarker::size + 4,
                height / 2 - Settings::ESP::Hitmarker::size - textHeight * i + 4
        );

        int damage = damages[i].first;
        std::string damageStr = '-' + std::to_string( damage );

        color.a = Color::FromImColor( Settings::ESP::Hitmarker::color.Color() ).a;
        color.a = std::min( color.a, ( int ) ( hitDiff * color.a / duration * 2 ) );

        Draw::Text( pos, damageStr.c_str(), esp_font, color );
    }
}

void Hitmarkers::FireGameEvent( IGameEvent* event ) {
    if ( !Settings::ESP::Hitmarker::enabled )
        return;

    if ( !engine->IsInGame() )
        return;

    if ( strcmp( event->GetName(), XORSTR( "player_hurt" ) ) != 0 )
        return;

    int hurt_player_id = event->GetInt( XORSTR( "userid" ) );
    int attacker_id = event->GetInt( XORSTR( "attacker" ) );

    if ( engine->GetPlayerForUserID( hurt_player_id ) == engine->GetLocalPlayer() )
        return;

    // TODO: show hitmarks while spectating?
    if ( engine->GetPlayerForUserID( attacker_id ) != engine->GetLocalPlayer() )
        return;

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer )
        return;

    C_BasePlayer* hurt_player = ( C_BasePlayer* ) entityList->GetClientEntity(
            engine->GetPlayerForUserID( hurt_player_id ) );
    if ( !hurt_player )
        return;

    if ( hurt_player->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Hitmarker::allies )
        return;

    if ( hurt_player->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Hitmarker::enemies )
        return;

    long now = Util::GetEpochTime();
    lastHitmarkerTimestamp = now;
    damages.insert( damages.begin(), std::pair<int, long>( event->GetInt( XORSTR( "dmg_health" ) ), now ) );
    if (Settings::ESP::Hitmarker::Sounds::enabled)
    {
        switch (Settings::ESP::Hitmarker::Sounds::sound)
        {
            case Sound::NONE:
                break;
            case Sound::METALDOOR2:
                engine->ClientCmd_Unrestricted("play doors\\hit_kickmetaldoor2");
                break;
            case Sound::DOORSTOP:
                engine->ClientCmd_Unrestricted("play doors\\default_stop");
                break;
            case Sound::METAL:
                engine->ClientCmd_Unrestricted("play physics\\metal\\metal_box_impact_bullet1");
                break;
            case Sound::DONTLEAVE:
                engine->ClientCmd_Unrestricted("play hostage\\hunuse\\dontleaveme");
                break;
            case Sound::THUNDER:
                engine->ClientCmd_Unrestricted("play ambient\\playonce\\weather\\thunder4");
                break;
            case Sound::BEEP:
                engine->ClientCmd_Unrestricted("play common\\beep");
                break;
            case Sound::BASS:
                engine->ClientCmd_Unrestricted("play common\\bass");
                break;
            case Sound::SK00TER:
                engine->ClientCmd_Unrestricted("play buttons\\arena_switch_press_02");
        }

    }
}
