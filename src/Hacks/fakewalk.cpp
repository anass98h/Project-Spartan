#include "fakewalk.h"

bool Settings::Fakewalk::enabled;
ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_C;



// this is still beta pls no hate :<


void Fakewalk::CreateMove( CUserCmd *cmd ) {
    C_BasePlayer *localplayer = ( C_BasePlayer * )entityList->GetClientEntity( engine->GetLocalPlayer( ) );
    if ( !localplayer || !localplayer->GetAlive( ) )
        return;

    if ( !Settings::Fakewalk::enabled )
        return;

    C_BaseCombatWeapon *activeWeapon = ( C_BaseCombatWeapon * )entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon( ) );
    if ( !activeWeapon || activeWeapon->GetInReload( ) )
        return;

    CSWeaponType weaponType = activeWeapon->GetCSWpnData( )->GetWeaponType( );
    if ( weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE ||
         weaponType == CSWeaponType::WEAPONTYPE_KNIFE )
        return;


    if ( inputSystem->IsButtonDown( Settings::Fakewalk::key ) ) {
        static int iChoked = -1;
        iChoked++;

        if ( iChoked < 1 ) {
            CreateMove::sendPacket = false;

            cmd->tick_count += 10;
            cmd->command_number += 7 + cmd->tick_count % 2 ? 0 : 1;

            cmd->buttons |= localplayer->GetMoveType( ) == IN_BACK;
            cmd->forwardmove = cmd->sidemove = 0.f;
        } else {
            CreateMove::sendPacket = true;
            iChoked = -1;

            globalVars->frametime *= ( localplayer->GetVelocity( ).Length2D( ) ) / 1.f;
            cmd->buttons |= localplayer->GetMoveType( ) == IN_FORWARD;
        }
    }


}
 
 
 







