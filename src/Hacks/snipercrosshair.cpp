#include "snipercrosshair.h"

bool Settings::SniperCrosshair::enabled = false;
ColorVar Settings::SniperCrosshair::color = ImColor( 255, 255, 255, 255 );

void SniperCrosshair::PaintImGui() {
    if ( !engine->IsInGame() )
        return;

    if ( !Settings::SniperCrosshair::enabled )
        return;

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer )
        return;

    if ( !localplayer->GetAlive() ) {
        C_BasePlayer* observerTarget = ( C_BasePlayer* ) entityList->GetClientEntityFromHandle(
                localplayer->GetObserverTarget() );
        if ( !observerTarget )
            return;

        localplayer = observerTarget;
    }

    C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon() );
    if ( !activeWeapon )
        return;

    if ( activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_SNIPER_RIFLE )
        return;

    if ( !localplayer->IsScoped() ) {
        int width;
        int height;
        engine->GetScreenSize( width, height );

        int middleHeight = height / 2;
        int middleWidth = width / 2;

        Draw::Line( Vector2D( middleWidth - 5, middleHeight ), Vector2D( middleWidth + 5, middleHeight ),
                    Color::FromImColor( Settings::SniperCrosshair::color.Color() ) );
        Draw::Line( Vector2D( middleWidth, middleHeight - 5 ), Vector2D( middleWidth, middleHeight + 5 ),
                    Color::FromImColor( Settings::SniperCrosshair::color.Color() ) );
    }


}
