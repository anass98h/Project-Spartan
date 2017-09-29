#include "fakewalk.h"

bool Settings::FakewalkAW::enabled;
ButtonCode_t Settings::FakewalkAW::key = ButtonCode_t::KEY_C;
bool Settings::Fakewalk::enabled;
ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_C;
bool Settings::ChokePackets::enabled;
ButtonCode_t Settings::ChokePackets::key = ButtonCode_t::KEY_X;
bool Settings::SlowMo::enabled;
ButtonCode_t Settings::SlowMo::key = ButtonCode_t::KEY_C;


ButtonCode_t W = ButtonCode_t::KEY_W;
ButtonCode_t A = ButtonCode_t::KEY_A;
ButtonCode_t S = ButtonCode_t::KEY_S;
ButtonCode_t D = ButtonCode_t::KEY_D;

bool Wpressed = inputSystem->IsButtonDown( W );
bool Apressed = inputSystem->IsButtonDown( A );
bool Spressed = inputSystem->IsButtonDown( S );
bool Dpressed = inputSystem->IsButtonDown( D );


void Fakewalk::CreateMove( CUserCmd* cmd ) {
    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer || !localplayer->GetAlive() )
        return;

    if ( Settings::FakewalkAW::enabled && inputSystem->IsButtonDown( Settings::FakewalkAW::key ) ) {
        if ( W )
            cmd->forwardmove = 10;

        if ( S )
            cmd->forwardmove = -10;

        if ( D )
            cmd->sidemove = 10;

        if ( A )
            cmd->sidemove = -10;

    } else if ( Settings::Fakewalk::enabled && inputSystem->IsButtonDown( Settings::Fakewalk::key ) ) {
        bool inAttack = cmd->buttons & IN_ATTACK;

        if ( !inAttack ) {
            if ( W ) {
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

            if ( S ) {
                static int ChokingPacketsS = -1;
                if ( ChokingPacketsS > 5 ) {
                    cmd->forwardmove = 1;
                    CreateMove::sendPacket = true;
                    ChokingPacketsS = -1;
                } else {
                    cmd->forwardmove = -35;
                    CreateMove::sendPacket = false;
                }
            }

            if ( A ) {
                static int ChokingPacketsA = -1;
                if ( ChokingPacketsA > 5 ) {
                    cmd->sidemove = 1;
                    CreateMove::sendPacket = true;
                    ChokingPacketsA = -1;
                } else {
                    cmd->sidemove = -35;
                    CreateMove::sendPacket = false;
                }
            }

            if ( D ) {
                static int ChokingPacketsD = -1;
                if ( ChokingPacketsD > 5 ) {
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
 







