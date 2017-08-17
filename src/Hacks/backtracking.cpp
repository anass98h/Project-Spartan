#include "backtracking.h"

std::map<int, std::deque<CTickRecord>> Backtracking::lagRecords;


void Backtracking::RestorePosition( int playerIndex, int tickDiff ) {
    return;
    C_BasePlayer *player = ( C_BasePlayer * )entityList->GetClientEntity( playerIndex );
}


/* Used for a Queue size-limit. Too lazy to write a wrapper class fam */
static inline void pushLagRecord( int index, C_BasePlayer *player = NULL ) {
    if ( player ) {
        Backtracking::lagRecords[index].push_back( CTickRecord( player ) );
    } else {
        Backtracking::lagRecords[index].push_back( CTickRecord( ) );
    }

    if ( Backtracking::lagRecords[index].size( ) > Backtracking::MAX_QUEUE_SIZE ) {
        Backtracking::lagRecords[index].pop_front( );
    }
}

/* Keep track of players */
void Backtracking::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !engine->IsInGame( ) ) {
        Backtracking::lagRecords.clear( ); // empty records.
        return;
    }
    if ( stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
        C_BasePlayer *me = ( C_BasePlayer * )entityList->GetClientEntity( engine->GetLocalPlayer( ) );
        if ( !me || !me->GetAlive( ) )
            return;
        for ( int i = 1; i < engine->GetMaxClients( ); ++i ) {
            C_BasePlayer *target = ( C_BasePlayer * )entityList->GetClientEntity( i );
            if ( !target ) {
                Backtracking::lagRecords.erase( i );
                continue;
            }
            if ( target == me || ( target->GetTeam( ) == me->GetTeam( ) ) )
                continue;
            if ( target->GetDormant( ) || !target->GetAlive( ) || target->GetImmune( ) ) {
                pushLagRecord( i, NULL );
                //push record with blank coords
            }
            pushLagRecord( i, target );
        }
    }
}
