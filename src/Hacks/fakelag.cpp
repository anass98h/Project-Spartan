#include "fakelag.h"

bool Settings::FakeLag::enabled = false;
int Settings::FakeLag::value = 9;
bool Settings::FakeLag::adaptive = false;
bool FakeLag::pSilent = false;
bool FakeLag::bFlipping = false;

static int ticks = 0;
static const int ticksMax = 16;

void FakeLag::CreateMove( CUserCmd* cmd ) {
    if ( !Settings::FakeLag::enabled )
        return;

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer || !localplayer->GetAlive() )
        return;

    if ( localplayer->GetFlags() & FL_ONGROUND && Settings::FakeLag::adaptive )
        return;

    if ( cmd->buttons & IN_ATTACK && !Settings::Aimbot::pSilent ) {
        CreateMove::sendPacket = true;
        return;
    }

    if ( FakeLag::pSilent ) {
        CreateMove::sendPacket = false;
        ticks++;
        return;
    }
    if ( ticks >= ticksMax && FakeLag::bFlipping ) {
        CreateMove::sendPacket = true;
        ticks = 0;
    } else {
        if ( Settings::FakeLag::adaptive ) // Adaptive lag is a piece of garbage. Was gonna rip it out, but perhaps something else will go here.
        {
            int packetsToChoke;
            if ( localplayer->GetVelocity().Length() > 0.f ) {
                packetsToChoke =
                        ( int ) ( ( 64.f / globalVars->interval_per_tick ) / localplayer->GetVelocity().Length() ) + 1;
                if ( packetsToChoke >= 15 )
                    packetsToChoke = 14;
                if ( packetsToChoke < Settings::FakeLag::value )
                    packetsToChoke = Settings::FakeLag::value;
            } else
                packetsToChoke = 0;

            CreateMove::sendPacket = ticks < 16 - packetsToChoke;
        } else {
            if ( ticks < 16 - Settings::FakeLag::value ) {
                if ( !FakeLag::bFlipping ) {
                    CreateMove::sendPacket = false;
                    ticks++;
                    return;
                }
                CreateMove::sendPacket = true;
            } else {
                CreateMove::sendPacket = false;
            }
        }
    }
    ticks++;
}
