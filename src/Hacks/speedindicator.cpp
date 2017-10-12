#include "speedindicator.h"

static const char wheel[] = "/tmp/wheel.png";
static const char speedo[] = "/tmp/speedometer.png";
static const char needle[] = "/tmp/needle.png";
static const char gas[] = "/tmp/gas.png";
static const char oil[] = "/tmp/oil.png";
static const char rearmirror[] = "/tmp/rearmirror.png";

static int width = 0;
static int height = 0;
static int engineWidth = 0;
static int engineHeight = 0;

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

    if ( width == 0 ) { /* Get the width/height needed for sizing calculations */
        width = (int)ImGui::GetWindowWidth();
        height = (int)ImGui::GetWindowHeight();
        engine->GetScreenSize(engineWidth, engineHeight);
    }

    if ( !Settings::SpeedIndicator::enabled || !engine->IsInGame() )
        return;

    C_BasePlayer* localPlayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localPlayer || !localPlayer->GetAlive() )
        return;
    C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            localPlayer->GetActiveWeapon() );

    /* Static Images */
    Draw::ImImage( speedo, ImVec2( ( width * 0.70f ), ( height * 0.75f ) ),
                   ImVec2( ( width * 0.70f ) + 210, ( height * 0.75f ) + 206 ) );
    Draw::ImImage( gas, ImVec2( ( width * 0.05f ), ( height * 0.75f ) ),
                   ImVec2( ( width * 0.05f ) + 225, ( height * 0.75f ) + 247 ) );
    Draw::ImImage( oil, ImVec2( ( width * 0.95f ) - 225, ( height * 0.75f ) ),
                   ImVec2( ( width * 0.95f ), ( height * 0.75f ) + 250 ) );
    Draw::ImImage( rearmirror, ImVec2( (width/2) - 266, 0), ImVec2( (width/2) + 266 , 224 ) );

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

void SpeedIndicator::RenderView(void* thisptr, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw) {
    static ITexture* mirrorTexture = NULL;
    static std::string textureName = XORSTR( "RearView" );
    static IMaterial *rearViewMat = NULL;
    static bool doFullUpdate = false;
    static long lastCalled = 0;
    if( !Settings::SpeedIndicator::enabled )
        return;
    if( Util::GetEpochTime() - lastCalled > 2000 ){ // Hack for it breaking on map change. might be false triggered by getting 0.5FPS.
        doFullUpdate = true;
        mirrorTexture = NULL;
        rearViewMat = NULL;
    }
    if( doFullUpdate ) {
        GetLocalClient( -1 )->m_nDeltaTick = -1;
        doFullUpdate = false;
    }
    C_BasePlayer* localPlayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localPlayer )
        return;
    if ( !localPlayer->GetAlive() ) {
        if ( *localPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && localPlayer->GetObserverTarget() )
            localPlayer = ( C_BasePlayer* ) entityList->GetClientEntityFromHandle( localPlayer->GetObserverTarget() );

        if ( !localPlayer )
            return;
    }
    if ( width == 0 ) {
        return; // Should be already set, but just in-case.
    }

    CViewSetup rearView = setup;
    rearView.x = rearView.oldX = 0;
    rearView.y = rearView.oldY = 0;
    rearView.width = rearView.oldWidth = 383;
    rearView.height = rearView.oldHeight = 88;
    rearView.angles = Vector(-setup.angles.x, (setup.angles.y-180), setup.angles.z);
    rearView.fov = 100.0f;
    rearView.m_flAspectRatio = float(rearView.width) / float(rearView.height);

    if( !mirrorTexture ) {
        material->forceBeginRenderTargetAllocation();
        mirrorTexture = material->createFullFrameRenderTarget( textureName.c_str(), rearView.width, rearView.height );
        //material->forceEndRenderTargetAllocation(); // Causes texture handles to go HAM. scrambles textures.
        if( !mirrorTexture ) {
            cvar->ConsoleDPrintf( XORSTR( "Could not Create Rear View Texture! (%s)\n" ), textureName.c_str() );
            return;
        }
    }

    IMatRenderContext *renderCtx = material->GetRenderContext();
    if( !renderCtx ){
        cvar->ConsoleDPrintf( XORSTR( "Could not acquire render context\n") );
        return;
    }

    renderCtx->PushRenderTargetAndViewport();
    renderCtx->SetRenderTarget(mirrorTexture);

    viewRenderVMT->GetOriginalMethod<RenderViewFn>( 6 )( thisptr, rearView, hudViewSetup,
                                                         VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_FULL_TARGET | VIEW_CLEAR_OBEY_STENCIL | VIEW_CLEAR_STENCIL,
                                                         0);
    if( !rearViewMat ) {
        std::stringstream materialData;
        materialData << "\"" << "UnlitGeneric" << "\"\n" <<
                     "{\n" <<
                     "\t\"$basetexture\" \"" << textureName.c_str() << "\"\n"
                             "}\n" << std::flush;

        std::string materialName = XORSTR( "KisakSucks" );
        KeyValues* keyValues = new KeyValues( XORSTR( "UnlitGeneric" ) );

        InitKeyValues( keyValues, XORSTR( "UnlitGeneric" ) );
        LoadFromBuffer( keyValues, materialName.c_str(), materialData.str().c_str(), nullptr, NULL, nullptr );

        rearViewMat = material->CreateMaterial( materialName.c_str(), keyValues );
        rearViewMat->AlphaModulate(1.0f);

        if( !rearViewMat ){
            cvar->ConsoleDPrintf( XORSTR( "Could not Create Rear View Material! (%s)\n" ), materialName.c_str() );
            renderCtx->Release();
            return;
        }
    }
    renderCtx->PopRenderTargetAndViewport();
    renderCtx->DrawScreenSpaceRectangle( rearViewMat, ((engineWidth/2) - (rearView.width/2)), 67, rearView.width, rearView.height,
                                        0.0f, 0.0f, float(rearView.width), float(rearView.height),
                                        mirrorTexture->GetActualWidth(), mirrorTexture->GetActualHeight(),
                                        NULL, 1, 1 );

    renderCtx->Release();

    lastCalled = Util::GetEpochTime();
}
