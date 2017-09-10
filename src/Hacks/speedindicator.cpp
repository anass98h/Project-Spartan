#include "speedindicator.h"

static const char wheel[] = "/tmp/wheel.png";
static const char speedo[] = "/tmp/speedometer.png";
static const char needle[] = "/tmp/needle.png";
static const char gas[] = "/tmp/gas.png";
static const char oil[] = "/tmp/oil.png";

bool Settings::SpeedIndicator::enabled = false;

int rotation_start_index;

/* carasuca's ImGui Rotation Hack: https://gist.github.com/carasuca/e72aacadcf6cf8139de46f97158f790f */
void ImRotateStart() {
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter() {
    ImVec2 l( FLT_MAX, FLT_MAX ), u( -FLT_MAX, -FLT_MAX ); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for ( int i = rotation_start_index; i < buf.Size; i++ )
        l = ImMin( l, buf[i].pos ), u = ImMax( u, buf[i].pos );

    return ImVec2( ( l.x + u.x ) / 2, ( l.y + u.y ) / 2 ); // or use _ClipRectStack?
}

ImVec2 operator-( const ImVec2& l, const ImVec2& r ) { return { l.x - r.x, l.y - r.y }; }

void ImRotateEnd( float rad, ImVec2 center = ImRotationCenter() ) {
    float s = sin( rad ), c = cos( rad );
    center = ImRotate( center, s, c ) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for ( int i = rotation_start_index; i < buf.Size; i++ )
        buf[i].pos = ImRotate( buf[i].pos, s, c ) - center;
}


void SpeedIndicator::PaintImGui() {
    if ( !Settings::SpeedIndicator::enabled || !engine->IsInGame() )
        return;

    C_BasePlayer* localPlayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localPlayer || !localPlayer->GetAlive() )
        return;
    C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            localPlayer->GetActiveWeapon() );

    static int width = 0;
    static int height = 0;
    if ( width == 0 ) {
        width = ImGui::GetWindowWidth();
        height = ImGui::GetWindowHeight();
    }

    /* Static Images */
    Draw::ImImage( speedo, ImVec2( ( width * 0.70f ), ( height * 0.75f ) ),
                   ImVec2( ( width * 0.70f ) + 210, ( height * 0.75f ) + 206 ) );
    Draw::ImImage( gas, ImVec2( ( width * 0.05f ), ( height * 0.75f ) ),
                   ImVec2( ( width * 0.05f ) + 225, ( height * 0.75f ) + 247 ) );
    Draw::ImImage( oil, ImVec2( ( width * 0.95f ) - 225, ( height * 0.75f ) ),
                   ImVec2( ( width * 0.95f ), ( height * 0.75f ) + 250 ) );

    /* Gas Needle*/
    ImRotateStart();
    Draw::ImImage( needle, ImVec2( ( width * 0.05f ) + 90, ( height * 0.75f ) + 113 ),
                   ImVec2( ( width * 0.05f ) + 90 + 42, ( height * 0.75f ) + 113 + 164 ) );
    ImRotateEnd( DEG2RAD( 175 - ( .63f * localPlayer->GetHealth() ) ) );

    /* Torque Needle*/
    ImRotateStart();
    Draw::ImImage( needle, ImVec2( ( width * 0.05f ) + 164, ( height * 0.75f ) + 17 ),
                   ImVec2( ( width * 0.05f ) + 164 + 39, ( height * 0.75f ) + 17 + 155 ) );
    if ( activeWeapon )
        ImRotateEnd( DEG2RAD( 182 - ( activeWeapon->GetInaccuracy() * 75.0f * 2.0f ) ) );
    else
        ImRotateEnd( DEG2RAD( 182 ) );

    /* Speedo Needle */
    ImRotateStart();
    Draw::ImImage( needle, ImVec2( ( width * 0.70f ) + 80, ( height * 0.75f ) + 7 ),
                   ImVec2( ( width * 0.70f ) + 130, ( height * 0.75f ) + 197 ) );
    if ( localPlayer->GetVelocity().Length2D() > 10.0f )
        ImRotateEnd( DEG2RAD( 243 - ( .80f * localPlayer->GetVelocity().Length2D() ) ) );
    else
        ImRotateEnd( DEG2RAD( 241 ) );

    /* Wheel */
    ImRotateStart();
    Draw::ImImage( wheel, ImVec2( ( width * 0.5f ) - ( width / 8 ), height - ( width / 8 ) ),
                   ImVec2( ( width * 0.5f ) + ( width / 8 ), height + ( width / 8 ) ) );
    ImRotateEnd( DEG2RAD( localPlayer->GetVAngles()->y ) );

    /* Oil needle */
    ImRotateStart();
    Draw::ImImage( needle, ImVec2( ( ( width * 0.95f ) - 225 ) + 61, ( height * 0.75f ) + 13 ),
                   ImVec2( ( ( width * 0.95f ) - 225 ) + 105, ( height * 0.75f ) + 178 ) );
    if ( activeWeapon ) // 117, 1
        ImRotateEnd( DEG2RAD( 183 -
                              ( ( 66.0f / activeWeapon->GetCSWpnData()->GetClipSize() ) * activeWeapon->GetAmmo() ) ) );
    else
        ImRotateEnd( DEG2RAD( 183 ) );

    // 142, 118 -- +44, +167

    /* Temp needle */
    ImRotateStart();
    Draw::ImImage( needle, ImVec2( ( ( width * 0.95f ) - 225 ) + 142, ( height * 0.75f ) + 118 ),
                   ImVec2( ( ( width * 0.95f ) - 225 ) + 142 + 44, ( height * 0.75f ) + 118 + 167 ) );
    // 175 - 110 (65)
    if ( activeWeapon ) {
        float nextAtk = ( activeWeapon->GetNextPrimaryAttack() - globalVars->curtime );
        if ( nextAtk > 1.0f )
            nextAtk = 1.0f;
        if ( nextAtk <= 0.1f )
            nextAtk = 0.01f;
        ImRotateEnd( DEG2RAD( 110.0f + ( 65.0f * nextAtk ) ) );
    } else
        ImRotateEnd( DEG2RAD( 175 ) );

}
