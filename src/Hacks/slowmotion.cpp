#include "slowmotion.h"

bool Settings::SlowMo::enabled = false;
ButtonCode_t Settings::SlowMo::key = ButtonCode_t::KEY_Z;

static bool flip = false;

void SlowMotion::CreateMove( CUserCmd* cmd ) {
    if ( Settings::SlowMo::enabled && inputSystem->IsButtonDown( Settings::SlowMo::key ) ) {

        C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
        if ( !pLocal || !pLocal->GetAlive() )
            return;

        C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
                pLocal->GetActiveWeapon() );
        if ( !activeWeapon || activeWeapon->GetInReload() )
            return;

        CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
        if ( weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE ||
             weaponType == CSWeaponType::WEAPONTYPE_KNIFE )
            return;

        if ( flip ) {
            cmd->tick_count = std::numeric_limits<int>::max();
        }

        flip = !flip;
    }
}
