#include "fakelag.h"

FakeLagType Settings::FakeLag::type = FakeLagType::OFF;
int Settings::FakeLag::value = 9;
bool FakeLag::bFlipping = false;

int cmdCounter = 0;
int stepCounter = 0;

void FakeLag::CreateMove( CUserCmd* cmd ) {
    if ( Settings::FakeLag::type == FakeLagType::OFF ) {
        return;
    }

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer || !localplayer->GetAlive() ) {
        return;
    }

    if ( Settings::BHop::enabled && ( cmd->buttons & IN_JUMP ) ) {
        *bSendPacket = true;
    }

    if ( ( cmd->buttons & IN_ATTACK ) && !Settings::Aimbot::pSilent ) {
        CreateMove::sendPacket = true;
        return;
    }

    C_BaseCombatWeapon* weapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon() );
    if ( !weapon ) {
        return;
    }

    if ( weapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_KNIFE ) {
        *bSendPacket = true;
        return;
    }

    switch ( Settings::FakeLag::type ) {
        case FakeLagType::TUX:
            if ( cmdCounter >= 16 && FakeLag::bFlipping ) {
                CreateMove::sendPacket = true;
                cmdCounter = 0;
            } else if ( cmdCounter < 16 - Settings::FakeLag::value ) {
                if ( !FakeLag::bFlipping ) {
                    CreateMove::sendPacket = false;
                    cmdCounter++;
                    return;
                }
                CreateMove::sendPacket = true;
            } else {
                CreateMove::sendPacket = false;
            }
            break;
        case FakeLagType::KMETH:
            if ( cmdCounter >= Settings::FakeLag::value ) {
                CreateMove::sendPacket = true;
                cmdCounter = 0;
            } else {
                CreateMove::sendPacket = false;
            }
            cmdCounter++;
            break;
        case FakeLagType::STEP:
            if ( cmdCounter >= stepCounter ) {
                stepCounter++;
                cmdCounter = 0;
                if ( stepCounter >= Settings::FakeLag::value ) {
                    CreateMove::sendPacket = true;
                    stepCounter = 0;
                }
            } else {
                CreateMove::sendPacket = false;
            }
            cmdCounter++;
            break;
        case FakeLagType::REACTIVE:
            if ( cmdCounter >= Settings::FakeLag::value - stepCounter ) {
                CreateMove::sendPacket = true;
                stepCounter--;
                cmdCounter = 0;
                if ( stepCounter <= 0 ) {
                    stepCounter = Settings::FakeLag::value;
                }
            } else {
                CreateMove::sendPacket = false;
            }
            cmdCounter++;
            break;
        case FakeLagType::OFF:
            // If this gets called something very weird has happened that shouldn't happen
            *( int* ) 0 = 0;
            break;
    }


}
