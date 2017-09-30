#include "fakewalk.h"

bool Settings::FakewalkAW::enabled;
ButtonCode_t Settings::FakewalkAW::key = ButtonCode_t::KEY_C;
bool Settings::Fakewalk::enabled;
ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_C;
bool Settings::ChokePackets::enabled;
ButtonCode_t Settings::ChokePackets::key = ButtonCode_t::KEY_X;
bool Settings::SlowMo::enabled;
ButtonCode_t Settings::SlowMo::key = ButtonCode_t::KEY_C;


void Fakewalk::CreateMove( CUserCmd* cmd ) {
    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer || !localplayer->GetAlive() )
        return;

    if ( Settings::FakewalkAW::enabled && inputSystem->IsButtonDown( Settings::FakewalkAW::key ) ) {
        bool isForward = cmd->buttons & IN_FORWARD;
        bool isBack = cmd->buttons & IN_BACK;
        bool isRight = cmd->buttons & IN_RIGHT;
        bool isLeft = cmd->buttons & IN_LEFT;

        if ( isForward )
            cmd->forwardmove = -10;

        if ( isBack )
            cmd->forwardmove = 10;

        if ( isRight )
            cmd->sidemove = 10;

        if ( isLeft )
            cmd->sidemove = -10;

    } else if ( Settings::Fakewalk::enabled && inputSystem->IsButtonDown( Settings::Fakewalk::key ) ) {
        bool isForward = cmd->buttons & IN_FORWARD;
        bool isBack = cmd->buttons & IN_BACK;
        bool isRight = cmd->buttons & IN_RIGHT;
        bool isLeft = cmd->buttons & IN_LEFT;
        bool inAttack = cmd->buttons & IN_ATTACK;

        if ( !inAttack ) {
            if ( isForward ) {
                static int ChokingPacketsW = -1;
                if ( ChokingPacketsW > 5 ) {
                    cmd->forwardmove = -1;
                    CreateMove::sendPacket = true;
                    ChokingPacketsW = -1;
                } else {
                    cmd->forwardmove = 35;
                    CreateMove::sendPacket = false;
                }
            }

            if ( isBack ) {
                static int ChokingPacketsS = -1;
                if ( ChokingPacketsS > 3 ) {
                    cmd->forwardmove = 1;
                    CreateMove::sendPacket = true;
                    ChokingPacketsS = -1;
                } else {
                    cmd->forwardmove = -35;
                    CreateMove::sendPacket = false;
                }
            }

            if ( isLeft ) {
                static int ChokingPacketsA = -1;
                if ( ChokingPacketsA > 3 ) {
                    cmd->sidemove = 1;
                    CreateMove::sendPacket = true;
                    ChokingPacketsA = -1;
                } else {
                    cmd->sidemove = -35;
                    CreateMove::sendPacket = false;
                }
            }

            if ( isRight ) {
                static int ChokingPacketsD = -1;
                if ( ChokingPacketsD > 3 ) {
                    cmd->sidemove = -1;
                    CreateMove::sendPacket = true;
                    ChokingPacketsD = -1;
                } else {
                    cmd->sidemove = 35;
                    CreateMove::sendPacket = false;
                }
            }
        }
    } else if ( Settings::ChokePackets::enabled && inputSystem->IsButtonDown( Settings::ChokePackets::key ) ) {
        static bool chokedPackets = 0;
        if ( chokedPackets > 10000 ) {
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
 







