#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::deadResolve = false;


void Resolver::Hug( C_BasePlayer* pEntity ) {
    if ( !Settings::Resolver::enabled )
        return;
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
            Hug ( aimbotTarget );
        else {
            for ( int i = 1; i < engine->GetMaxClients(); ++i ) {
                C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

                if ( !target
                     || target == pLocal
                     || target->GetDormant()
                     || target->GetAlive()
                     || target->GetImmune()
                     || target->GetTeam() == pLocal->GetTeam() )
                    continue;

                Hug ( target );
            }
        }
    }
}

void Resolver::PostFrameStageNotify( ClientFrameStage_t stage ) {
}

void Resolver::CreateMove( CUserCmd* cmd ) {
}