#include "fakewalk.h"

bool Settings::FakewalkAW::enabled;
ButtonCode_t Settings::FakewalkAW::key = ButtonCode_t::KEY_C;
bool Settings::Fakewalk::enabled;
ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_Y;
bool Settings::ChokePackets::enabled;
ButtonCode_t Settings::ChokePackets::key = ButtonCode_t::KEY_X;
bool Settings::SlowMo::enabled;
ButtonCode_t Settings::SlowMo::key = ButtonCode_t::KEY_C;

void Fakewalk::CreateMove( CUserCmd* cmd ) {
    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer || !localplayer->GetAlive() )
        return;

    if ( Settings::FakewalkAW::enabled && inputSystem->IsButtonDown( Settings::FakewalkAW::key ) ) {
      static int iChoked = -1;
      iChoked++;

      if ( iChoked < 1 )
      {
        CreateMove::sendPacket = false;

        cmd->tick_count += 10;
        cmd->command_number += 7 + cmd->tick_count % 2 ? 0 : 1;

        cmd->buttons |= localplayer->GetMoveType() == IN_BACK;
        cmd->forwardmove = cmd->sidemove = 0.f;
      } else {
        CreateMove::sendPacket = true;
        iChoked = -1;

        globalVars->frametime *= ( localplayer->GetVelocity().Length2D() ) / 1.f;
        cmd->buttons |= localplayer->GetMoveType() == IN_FORWARD;
      }
    } else if ( Settings::Fakewalk::enabled && inputSystem->IsButtonDown( Settings::Fakewalk::key ) ) {
        bool inAttack = cmd->buttons & IN_ATTACK;

        static int chokedPackets = 0;
        if ( !inAttack ) {
            chokedPackets++;
            if ( chokedPackets > 6 ) {
                cmd->sidemove = cmd->forwardmove = 0;
            } else {
                if ( chokedPackets > 9 ) {
                    CreateMove::sendPacket = true;
                    chokedPackets = 0;
                } else {
                    CreateMove::sendPacket = false;
                    if ( localplayer->GetVelocity().Length2D() > 15.f ) {
                        cmd->sidemove = cmd->forwardmove = 0;
                    }
                }
            }
        }
    } else if ( Settings::ChokePackets::enabled && inputSystem->IsButtonDown( Settings::ChokePackets::key ) ) {
        static bool chokedPackets = 0;
        if ( chokedPackets > 1000 ) {
            CreateMove::sendPacket = true;
            chokedPackets = 0;
        } else {
            CreateMove::sendPacket = false;
            chokedPackets++;
        }
    } else if ( Settings::SlowMo::enabled ) {

        C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
                localplayer->GetActiveWeapon() );
        if ( !activeWeapon || activeWeapon->GetInReload() )
            return;

        CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
        if ( weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE ||
             weaponType == CSWeaponType::WEAPONTYPE_KNIFE )
            return;


        if ( inputSystem->IsButtonDown( Settings::SlowMo::key ) ) {
            static bool slowmo;
            slowmo = !slowmo;
            if ( slowmo ) {
                cmd->tick_count = INT_MAX;
            }
        }
    } else
        return;
}
