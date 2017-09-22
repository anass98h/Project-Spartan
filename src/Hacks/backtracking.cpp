#include "backtracking.h"

std::map<int, std::deque<CTickRecord>> Backtracking::lagRecords;
float previousInterp = -1.0f;
int previousInterpolate = -1;
int previousLagCompensation = -1;

C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

void CLagCompensation2::SaveNetvars(StoredNetvars *dest, C_BasePlayer *player) {
    dest->origin = player->GetVecOrigin();

    dest->flags = player->GetFlags();
    dest->simulationtime = player->GetSimulationTime();
    dest->min = player->GetMin();
    dest->max = player->GetMax();
    dest->lby = *player->GetLowerBodyYawTarget();
    dest->eyeangles = *player->GetEyeAngles();

    dest->sequence = player->GetSequence();
    dest->cycle = player->GetCycle();

    dest->velo = player->GetVelocity();

    std::array<float, 24> p = player->GetPoseParameter();
    std::array<float, 24> save = dest->poseparam;

    //std::memcpy(dest->poseparam, pEntity->GetPosePosition(), 24 * sizeof(float));
    std::memcpy(&save, &p, 24 * sizeof(float));

    //not big deal proper anim fix
}

void CLagCompensation2::RestoreNetvars(StoredNetvars *src, C_BasePlayer *player) {
    *player->GetVecOriginPointer() = src->origin;

    *player->GetFlagsPointer() = src->flags;
    *player->GetSimulationTimePointer() = src->simulationtime;
    *player->GetMinPointer() = src->min;
    *player->GetMaxPointer() = src->max;
    *player->GetLowerBodyYawTarget() = src->lby;
    *player->GetEyeAngles() = src->eyeangles;

    *player->GetSequencePointer() = src->sequence;
    *player->GetCyclePointer() = src->cycle;

    *player->GetVelocityPointer() = src->velo;

    std::array<float, 24> save = src->poseparam;
    std::array<float, 24> p = player->GetPoseParameter();

    //std::memcpy(player->GetPoseParameter(), src->poseparam, 24 * sizeof(float)); //skeet way of doing shit
    std::memcpy(&p, &save, 24 * sizeof(float)); //skeet way of doing shit
}

float CLagCompensation2::GetLerpTime() {
    float updaterate = cvar->FindVar(XORSTR("cl_updaterate"))->GetFloat();
    float ratio = cvar->FindVar(XORSTR("cl_interp_ratio"))->GetFloat();
    float lerp = cvar->FindVar(XORSTR("cl_interp"))->GetFloat();

    return std::max(lerp, ratio / updaterate);
}

float CLagCompensation2::GetLatency(int type) {
    INetChannelInfo *nci = engine->GetNetChannelInfo();

    if (nci) {
        return nci->GetLatency(type);
    }

    return 0.f;
}

bool CLagCompensation2::isValidTick(int tick) {
    if (TICKS_TO_TIME(pLocal->GetTickBase() - tick) < Math::Clamp((.2f + GetLatency(FLOW_INCOMING) - GetLatency(FLOW_OUTGOING)), 0.f, .9f))
        return true;

    return false;
}

void CLagCompensation2::SetValidTickCount(C_BasePlayer* player, CUserCmd* pCmd) {
    int iEntityId = player->GetIndex();

    if (Settings::Resolver::LagComp && pRecordRollback[iEntityId]) {
        pCmd->tick_count = TIME_TO_TICKS(pRecordRollback[iEntityId]->simulationtime + GetLerpTime());
    }
    else {
        pCmd->tick_count = TIME_TO_TICKS(player->GetSimulationTime() + GetLerpTime());
    }
}

void CLagCompensation2::StoreDatas(C_BasePlayer* player) {
    int iEntityId = player->GetIndex();

    for (int j = 0; j < vecLagRecord[iEntityId].size(); ++j) {
        if (!isValidTick(TIME_TO_TICKS(vecLagRecord[iEntityId][j].simulationtime + GetLerpTime()))) {
            vecLagRecord[iEntityId].erase(vecLagRecord[iEntityId].begin() + j);
            break;
        }
    }

    //LOWERBODYUPDATE
    static float flOldLowerBody[64];

    float flCurLowerBody = *player->GetLowerBodyYawTarget();

    bool bLbyUpdate = false;

    if (flOldLowerBody[iEntityId] != flCurLowerBody) {
        flOldLowerBody[iEntityId] = flCurLowerBody;
        bLbyUpdate = true;
    }

    //CORE
    static float flOldSimTime[64];

    float flCurSimTime = player->GetSimulationTime();

    if (fabs(flOldSimTime[player->GetIndex()] - flCurSimTime) > 5.f)
        flOldSimTime[player->GetIndex()] = 0.f;

    StoredNetvars LagRecord;

    if (flOldSimTime[iEntityId] < flCurSimTime) {
        SaveNetvars(&LagRecord, player);
        LagRecord.lbyupdate = bLbyUpdate;

        vecLagRecord[iEntityId].emplace_back(LagRecord);

        flOldSimTime[iEntityId] = flCurSimTime;
    }
}

int CLagCompensation2::GetLastLbyRecord(C_BasePlayer* player) {
    int iEntityId = player->GetIndex();

    if (!pLocal)
        return false;

    if (player->GetTeam() == pLocal->GetTeam())
        return -1;

    if (vecLagRecord[iEntityId].empty())
        return -1;

    int result = -1;

    for (int i = 0; i < vecLagRecord[iEntityId].size(); ++i) {
        if (vecLagRecord[iEntityId][i].lbyupdate)
            result = i;
    }

    return result;
}

void CLagCompensation2::RestorePlayer(C_BasePlayer* player) {
    //if (!Settings::Resolver::LagComp)
    //    return;

    static bool isMoving = false;

    if (fabsf(player->GetVelocity().Length2D()))
        isMoving = true;
    else
        isMoving = false;

    if (vecLagRecord[player->GetIndex()].empty())
        return;

    int bestLbyTarget = GetLastLbyRecord(player);
    StoredNetvars *src;
    if (bestLbyTarget != -1 && !isMoving) {
        src = &vecLagRecord[player->GetIndex()][bestLbyTarget];
    }
    else {
        src = &vecLagRecord[player->GetIndex()].back();
    }

    pRecordRollback[player->GetIndex()] = src;

    RestoreNetvars(src, player);
}

bool CLagCompensation2::isLbyUpdate(C_BasePlayer* player) {
    //if (!MenuOptions.ESP.g_lbyUpdate)
    //return false;

    int iEntityId = player->GetIndex();

    if (player->GetTeam() == pLocal->GetTeam())
        return false;

    if (vecLagRecord[iEntityId].empty())
        return false;

    for (int i = 0; i < vecLagRecord[iEntityId].size(); ++i) {
        if (vecLagRecord[iEntityId][i].lbyupdate)
            return true;
    }

    return false;
}

void CLagCompensation2::Store(C_BasePlayer *player) {
    CLagCompensation2 lagComp;
    lagComp.StoreDatas(player);
}

void CLagCompensation2::Restore(C_BasePlayer *player) {
    CLagCompensation2 lagComp;
    lagComp.RestorePlayer(player);
}

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
            PushLagRecord( i, target );
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
