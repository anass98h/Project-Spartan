#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::resolvePitch = false;
bool Settings::Resolver::lagCompensation = false;
bool Settings::Resolver::headshotLbyUpdateOnly = false;

bool Settings::SmartAim::enabled = false;
int Settings::SmartAim::baimAfterMissed = 3;

bool Settings::AngleFlip::enabled = false;
ButtonCode_t Settings::AngleFlip::key = ButtonCode_t::KEY_F;

// Variables
bool Resolver::lbyUpdated = false;

void Resolver::Hug( C_BasePlayer* target ) {
    if ( Settings::Resolver::resolvePitch )
        HugPitch( target );
    
    QAngle angle = *target->GetEyeAngles();

    *target->GetEyeAngles()->y = angle.y;
}

void Resolver::HugLby( C_BasePlayer* target ) {
    // Do your magic Myrrib & Rasp
}

void Resolver::HugBrute( C_BasePlayer* target ) {
    float lby = target->GetLowerBodyYawTarget();
}

void Resolver::HugPitch( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    *target->GetEyeAngles()->x = angle.x;
}

bool Resolver::LbyUpdated( C_BasePlayer* target ) {
    float velocity = target->GetVelocity().Length2D();
    bool onGround = target->GetFlags() & ON_GROUND;
    bool isMoving = onGround && velocity > 35.f;
    float serverTime = target->GetTickBase() * globalVars->interval_per_tick;
    float lastLbyUpdate = 0;

    if ( isMoving || serverTime == lastLbyUpdate + 1.1f )
        Resolver::lbyUpdated = true;
    else
        Resolver::lbyUpdated = false;

    if ( Resolver::lbyUpdated ) {
        lastLbyUpdate = serverTime;
        return true;
    } else
        return false;
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
