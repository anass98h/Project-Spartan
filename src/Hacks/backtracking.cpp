#include "backtracking.h"

std::map<int, std::deque<CTickRecord>> Backtracking::lagRecords;
float previousInterp = -1.0f;
int previousInterpolate = -1;
int previousLagCompensation = -1;
bool Backtracking::backtrackingLby = false;

void Backtracking::RestorePosition( int playerIndex, int tickDiff ) {
    return;
}

/* Used for a Queue size-limit. Too lazy to write a wrapper class fam */
static inline void PushLagRecord( int index, C_BasePlayer* player = NULL ) {
    if ( player ) {
        Backtracking::lagRecords[index].push_back( CTickRecord( player ) );
    } else {
        Backtracking::lagRecords[index].push_back( CTickRecord() );
    }

    if ( Backtracking::lagRecords[index].size() > Backtracking::MAX_QUEUE_SIZE ) {
        Backtracking::lagRecords[index].pop_front();
    }
}

/* Keep track of players */
void Backtracking::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !engine->IsInGame() ) {
        Backtracking::lagRecords.clear(); // empty records.
        return;
    }

    if ( stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
        C_BasePlayer* me = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
        if ( !me || !me->GetAlive() )
            return;
        for ( int i = 1; i < engine->GetMaxClients(); ++i ) {
            C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );
            if ( !target ) {
                Backtracking::lagRecords.erase( i );
                continue;
            }
            if ( target == me || ( target->GetTeam() == me->GetTeam() ) )
                continue;
            if ( target->GetDormant() || !target->GetAlive() || target->GetImmune() ) {
                PushLagRecord( i, NULL );
                //push record with blank coords
            }
            if ( Settings::Resolver::LagComp && Settings::Resolver::enabled ) {
                INetChannelInfo *nci = engine->GetNetChannelInfo();

                float outgoingPing = nci->GetLatency(FLOW_OUTGOING);

                static float lastUpdate = 0.f;

                if ( Resolver::lbyUpdated ) {
                    PushLagRecord( i, target );
                    lastUpdate = curTime + 0.2f + outgoingPing;
                    Backtracking::backtrackingLby = true;
                } else if ( lastUpdate < curTime ) {
                    PushLagRecord( i, target );
                    Backtracking::backtrackingLby = false;
                }
            } else {
                PushLagRecord( i, target );
            }
        }
    }
}

void Backtracking::ToggleRequiredCVars( bool activate ) {
    ConVar* interp = cvar->FindVar( XORSTR( "cl_interp" ) );
    ConVar* interpolate = cvar->FindVar( XORSTR( "cl_interpolate" ) );
    ConVar* lagCompensation = cvar->FindVar( XORSTR( "cl_lagcompensation" ) );

    if ( activate ) {
        if ( interp->GetFloat() != 0.0f ) {
            previousInterp = interp->GetFloat();
            interp->SetValue( 0.0f );
        }

        if ( interpolate->GetInt() != 0 ) {
            previousInterpolate = interpolate->GetInt();
            interpolate->SetValue( 0 );
        }

        if ( lagCompensation->GetInt() != 1 ) {
            previousLagCompensation = lagCompensation->GetInt();
            lagCompensation->SetValue( 1 );
        }
    } else {
        if ( previousInterp != -1.0f ) {
            interp->SetValue( previousInterp );
        }

        if ( previousInterpolate != -1 ) {
            interpolate->SetValue( previousInterpolate );
        }

        if ( previousLagCompensation != -1 ) {
            lagCompensation->SetValue( previousLagCompensation );
        }
    }
}
