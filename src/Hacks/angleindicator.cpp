#include "angleindicator.h"

bool Settings::AngleIndicator::enabled = false;
bool Settings::AngleIndicator::Veloc = false;
static float fakeAngle;
static float realAngle;
static float velocity;

void AngleIndicator::PostPredictionCreateMove( CUserCmd* cmd ) {

    if ( !engine->IsInGame() )
        return;

    if ( !Settings::AngleIndicator::enabled && !Settings::AngleIndicator::Veloc )
        return;


    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() )
        return;

    if ( ( AntiAim::IsAirborne() ? Settings::AntiAim::Airborne::Yaw::enabled :
           AntiAim::IsMoving() ? Settings::AntiAim::Moving::Yaw::enabled :
           Settings::AntiAim::Standing::Yaw::enabled ) ) {
        fakeAngle = AntiAim::lastFakeYaw;
        realAngle = AntiAim::lastRealYaw;
        velocity = pLocal->GetVelocity().Length2D();
    } else {
        fakeAngle = 0;
        realAngle = cmd->viewangles.y;
        velocity = pLocal->GetVelocity().Length2D();
    }
}

void AngleIndicator::PaintImGui() {
    if ( !engine->IsInGame() )
        return;

    if ( !Settings::AngleIndicator::enabled && !Settings::AngleIndicator::Veloc )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() )
        return;

    // Creation of the text

    std::ostringstream stream;
    stream << Math::RoundFloat( realAngle );
    std::string angleText( stream.str() );

    std::string text( XORSTR( "Real: " ) );
    text.append( angleText );

    std::ostringstream stream2;
    stream2 << Math::RoundFloat( fakeAngle );
    std::string fakeText( stream2.str() );
    std::string text2( XORSTR( "Fake: " ) );
    text2.append( fakeText );

    std::ostringstream stream3;
    stream3 << Math::RoundFloat( *pLocal->GetLowerBodyYawTarget() );
    std::string lbyText( stream3.str() );
    std::string text3( XORSTR( "LBY: " ) );
    text3.append( lbyText );

    std::ostringstream stream4;
    stream4 << Math::RoundFloat( velocity );
    std::string veloText( stream4.str() );
    std::string text4( XORSTR( "Velocity: " ) );
    text4.append( veloText );
    // Calculation of text position on screen

    std::ostringstream stream5;
    stream5 << (AntiAim::IsStanding() ? XORSTR("Standing") : (AntiAim::IsMoving() ? XORSTR("Moving") :
                                                      (AntiAim::IsAirborne() ? XORSTR("Airborne") : XORSTR("Unknown"))));
    std::string stateText( stream5.str() );
    std::string text5(XORSTR("Current State: "));
    text5.append(stateText);

    int width;
    int height;
    engine->GetScreenSize( width, height );

    int textX = ( width - ( width - ( width * 85 / 100 ) ) );
    int textY = height - ( height - ( height * 53 / 100 ) );
    int textX2 = ( width - ( width - ( width * 85 / 100 ) ) );
    int textY2 = height - ( height - ( height * 51 / 100 ) );
    int textX3 = ( width - ( width - ( width * 85 / 100 ) ) );
    int textY3 = height - ( height - ( height * 55 / 100 ) );
    int textX4 = ( width - ( width - ( width * 85 / 100 ) ) );
    int textY4 = height - ( height - ( height * 57 / 100 ) );
    int textX5 = ( width - ( width - ( width * 85 / 100 ) ) );
    int textY5 = height - ( height - ( height * 59 / 100 ) );
    Draw::ImText( ImVec2( textX, textY ), ImColor( 255, 255, 255 ), text.c_str(), NULL, 0.0f, NULL,
                  ImFontFlags_Shadow );
    Draw::ImText( ImVec2( textX2, textY2 ), ImColor( 255, 255, 255 ), text2.c_str(), NULL, 0.0f, NULL,
                  ImFontFlags_Shadow );
    Draw::ImText( ImVec2( textX3, textY3 ), ImColor( 255, 255, 255 ), text3.c_str(), NULL, 0.0f, NULL,
                  ImFontFlags_Shadow );
    if ( Settings::AngleIndicator::Veloc ) {
        Draw::ImText( ImVec2( textX4, textY4 ), ImColor( 255, 255, 255 ), text4.c_str(), NULL, 0.0f, NULL,
                      ImFontFlags_Shadow );
        Draw::ImText( ImVec2( textX5, textY5 ), ImColor(255, 255, 255), text5.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
    }
}
