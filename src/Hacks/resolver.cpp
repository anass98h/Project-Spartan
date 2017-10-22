#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::resolvePitch = false;
bool Settings::Resolver::lagCompensation = false;
bool Settings::Resolver::headshotLbyUpdateOnly = false;

bool Settings::SmartAim::enabled = false;
int Settings::SmartAim::baimAfterMissed = 3;

bool Settings::AngleFlip::enabled = false;
ButtonCode_t Settings::AngleFlip::key = ButtonCode_t::KEY_F;

void Resolver::Hug( C_BasePlayer* target ) {
    //QAngle angle = *target->GetEyeAngles();
    if(target->GetVelocity().Length2D() > 35 && target->GetFlags() & FL_ONGROUND) // TODO: Add fakewalk check
        target->GetEyeAngles()->y = *target->GetLowerBodyYawTarget();
    else
    {
        if(target->GetFlags() & FL_ONGROUND)


    }


}

void Resolver::HugLby( C_BasePlayer* target ) {
    // Do your magic Myrrib & Rasp
}

void Resolver::HugBrute( C_BasePlayer* target ) {
    // Do your magic Myrrib & Rasp
}

void Resolver::HugPitch( C_BasePlayer* target ) {
    // Do your magic Myrrib & Rasp
}

////////////////////////////////////////////////
/// HOOKS
////////////////////////////////////////////////

void Resolver::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !Settings::Resolver::enabled || !engine->IsInGame() ) {
        return;
    }

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() ) {
        return;
    }

    switch ( stage ) {
        case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START:
            // Do your magic Myrrib & Rasp
            break;
        case ClientFrameStage_t::FRAME_RENDER_END:
            // Do your magic Myrrib & Rasp
            break;
    }
}

void Resolver::PostFrameStageNotify( ClientFrameStage_t stage ) {
    // Do your magic Myrrib & Rasp
}

void Resolver::CreateMove( CUserCmd* cmd ) {
    // Do your magic Myrrib & Rasp
}

void Resolver::Paint() {
    // This is if you want to paint some resolver info on screen, if you don't want it just remove it from here or leave
    // it blank.
}
