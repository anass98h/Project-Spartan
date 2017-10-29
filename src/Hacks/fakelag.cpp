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
        case FakeLagType::NUCLEAR: {
                // float counter = Settings::FakeLag::value; Unused
                static int chokedTicks = 0;
                static int maxChokedTicks = 0;
                float speed = localplayer->GetVelocity().Length2D();
                // float multiplier = 0.9375; Unused

                if ( speed > 350 )
                    maxChokedTicks = 10;
                else if ( speed > 500 )
                    maxChokedTicks = 8;
                else if ( speed > 750 )
                    maxChokedTicks = 6;
                else
                    maxChokedTicks = 15;

                if ( chokedTicks > maxChokedTicks ) {
                    CreateMove::sendPacket = true;
                    chokedTicks = 0;
                } else {
                    CreateMove::sendPacket = false;
                    chokedTicks++;
                }
            }
            break;
        case FakeLagType::LUNICO:
            static int chokeAmount;

            if ( localplayer->GetVelocity().Length2D() > 0.1f ) {
                chokeAmount = static_cast<int>(( 64.0f / globalVars->interval_per_tick ) /
                                               localplayer->GetVelocity().Length2D());
            } else {
                chokeAmount = 16;
            }

            if ( chokeAmount > 16 ) {
                chokeAmount = 16;
            }

            cmdCounter++;

            if ( cmdCounter >= chokeAmount ) {
                CreateMove::sendPacket = true;
                cmdCounter = 0;
            } else {
                CreateMove::sendPacket = false;
            }
            break;
        case FakeLagType::OFF:
            // It's not possible to get to this point.
            break;
    }

}
