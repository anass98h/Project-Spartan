#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::deadResolve = false;
bool Settings::Resolver::pitch = false;

bool Resolver::lbyUpdated = false;

void Resolver::ResolveY( C_BASePlayer* pEntity ) {
    if ( !Settings::Resolver::pitch )
        return;

    QAngle angle = *pEntity->GetViewAngles();

    *pEntity->GetViewAngles()->x = angle.x;
}

void Resolver::Hug( C_BasePlayer* pEntity ) {
    if ( !Settings::Resolver::enabled )
        return;

    if ( Settings::Resolver::pitch )
        ResolveY( pEntity );

    QAngle angle = *pEntity->GetViewAngles();

    float lby = *pEntity->GetLowerBodyYawTarget();
    float serverTime = pEntity->GetTickBase() * globalVars->interval_per_tick;
    float velocity = pEntity->GetVelocity().Lenght2D();
    bool onGround = pEnity->GetFlags() & FL_ONGROUND;
    bool isMoving = onGround && velocity > 0.1;

    if ( isMoving )
        Resolver::lbyUpdated = true;
    else
        Resolver::lbyUpdated = false;

    *pEntity->GetViewAngles()->y = angle.y;
}

void Resolver::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !engine->IsInGame() || !Settings::Resolver::enabled )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    if( !Settings::Resolver::deadResolve ) {
        if ( !pLocal || !pLocal->GetAlive() )
            return;
    }

    if ( stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
        C_BasePlayer* aimbotTarget = ( C_BasePlayer* ) entityList->GetClientEntity( Aimbot::targetAimbot );

        if ( Aimbot::useAbTarget )
            Hug( aimbotTarget );
        else {
            for ( int i = 1; i < engine->GetMaxClients(); i++ ) {
                C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

                if ( !target
                     || target == pLocal
                     || target->GetDormant()
                     || target->GetAlive()
                     || target->GetImmune()
                     || target->GetTeam() == pLocal->GetTeam() )
                    continue;

                Hug( target );
            }
        }
    }
}

void Resolver::PostFrameStageNotify( ClientFrameStage_t stage ) {
}

void Resolver::CreateMove( CUserCmd* cmd ) {
}