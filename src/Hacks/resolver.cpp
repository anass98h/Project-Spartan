#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::pitch = false;
float Settings::Resolver::ticks = 2;
float Settings::Resolver::modulo = 2;
bool Settings::Resolver::LagComp = false;
ResolverHugtype Settings::Resolver::Hugtype = ResolverHugtype::OFF;
std::vector<int64_t> Resolver::playerAngleLogs = {};
std::array<CResolveInfo, 32> Resolver::m_arrInfos;

int Shotsmissed = 0;
bool shotATT;
std::vector<std::pair<C_BasePlayer*, QAngle>> player_data;
std::random_device rd;
static float rSimTime[65];
LagComp Lagcomp;

static void StartLagComp(C_BasePlayer* ribrib) {
  if (!engine->IsInGame())
      return;

    if (Settings::Resolver::Hugtype != ResolverHugtype::BACKTRACKLBY)
        Settings::Resolver::Hugtype = ResolverHugtype::BACKTRACKLBY;

    if (Settings::Aimbot::backtrack)
       Settings::Aimbot::backtrack = false;


    Lagcomp.StoreData(ribrib);
    Lagcomp.RestorePlayer(ribrib);
    /*if (!engine->IsInGame()) {
        ConVar* interp = cvar->FindVar( XORSTR( "cl_interp" ) );
        ConVar* interpolate = cvar->FindVar( XORSTR( "cl_interpolate" ) );
        ConVar* lagcomp = cvar->FindVar( XORSTR( "cl_lagcompensation" ) );

        if (interp->GetInt() != 0)
            interp->SetValue("0");

        if (interpolate->GetInt() != 0)
            interpolate->SetValue("0");

        if (lagcomp->GetInt() != 1)
            lagcomp->SetValue("1");
    }*/

}

void LagComp::Store(StoredNetvars* dest, C_BasePlayer* player) {
    if (!engine->IsInGame())
        return;

    C_BasePlayer* me = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    if (!me || !player)
        return;

    dest->origin = player->GetVecOrigin();

    dest->flags = player->GetFlags();
    dest->simulationTime = player->GetSimulationTime();
    dest->min = player->GetMinPrescaled();
    dest->max = player->GetMaxPrescaled();
    dest->lby = *player->GetLowerBodyYawTarget();
    dest->eyeAngles = *player->GetEyeAngles();

    dest->sequence = player->GetSequence();
    dest->cycle = player->GetCycle();

    dest->velocity = player->GetVelocity();

    //fuck off animations, animations are gay, plz die animations :kys:
    //std::memcpy(dest->poseParam, player->GetPoseParameter(), 24 * sizeof(float));

    //pfix instead of using memcpy, which causes too much trouble for me
    /*for (int i = 0; i < 24; i++) {
        dest->poseParam[i] = player->GetPoseParameter()[i];
    }*/

    //not big deal proper anim fix
}

void LagComp::Restore(StoredNetvars* src, C_BasePlayer* player) {
    if (!engine->IsInGame())
        return;

    C_BasePlayer* me = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    if (!me || !player)
        return;

    player->GetVecOrigin() = src->origin;

    *player->GetFlagsPointer() = src->flags;
    *player->GetSimulationTimePointer() = src->simulationTime;
    player->GetMinPrescaled() = src->min;
    player->GetMaxPrescaled() = src->max;
    *player->GetLowerBodyYawTarget() = src->lby;
    *player->GetEyeAngles() = src->eyeAngles;

    *player->GetSequencePointer() = src->sequence;
    *player->GetCyclePointer() = src->cycle;

    player->GetVelocity() = src->velocity;

    //fuck off animations, animations are gay, plz die animations :kys:
    //std::memcpy(pEntity->GetPosePosition(), src->poseParam, 24 * sizeof(float)); //skeet way of doing shit

    //pfix instead of using memcpy, which causes too much trouble for me
    /*for (int i = 0; i < 24; i++) {
        player->GetPoseParameter()[i] = src->poseParam[i];
    }*/
}

float LagComp::GetLerpTime() {
    float updaterate = cvar->FindVar(XORSTR("cl_updaterate"))->GetFloat();
    float ratio = cvar->FindVar(XORSTR("cl_interp_ratio"))->GetFloat();
    float lerp = cvar->FindVar(XORSTR("cl_interp"))->GetFloat();

    return std::max(lerp, ratio / updaterate);
}
float LagComp::GetLatency(int type) {
    INetChannelInfo *nci = engine->GetNetChannelInfo();

    if (nci) {
        return nci->GetLatency(type);
    }

    return 0.f;
}

bool LagComp::isValidTick(int tick , C_BasePlayer* pLocal) {
    float t1 = pLocal->GetTickBase() - tick;


    if (TICKS_TO_TIME( t1 ) < Math::Clamp((.2f + GetLatency(FLOW_INCOMING) - GetLatency(FLOW_OUTGOING)), 0.f, .9f) )
        return true;

    return false;
}

void LagComp::SetValidTickCount(C_BasePlayer* pEntity, CUserCmd* pCmd) {
    int iEntityId = pEntity->GetIndex();

    if (Settings::Resolver::LagComp && pRecordRollback[iEntityId]) {
        pCmd->tick_count = TIME_TO_TICKS(pRecordRollback[iEntityId]->simulationTime + GetLerpTime());
    }
    else {
        pCmd->tick_count = TIME_TO_TICKS(pEntity->GetSimulationTime() + GetLerpTime());
    }
}
void LagComp::StoreData(C_BasePlayer *player) {
    if (!engine->IsInGame())
        return;

    C_BasePlayer* me = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    if (!me || !player)
        return;

    int iEntityId = player->GetIndex();

    for (int j = 0; j < vecLagRecord[iEntityId].size(); ++j) {
        if (!isValidTick(TIME_TO_TICKS(vecLagRecord[iEntityId][j].simulationTime + GetLerpTime()), player)) {
            vecLagRecord[iEntityId].erase(vecLagRecord[iEntityId].begin() + j);
            break;
        }
    }

    //CORE
    static float flOldSimTime[64];

    float flCurSimTime = player->GetSimulationTime();

    if (fabs(flOldSimTime[iEntityId] - flCurSimTime) > 5.f)
        flOldSimTime[iEntityId] = 0.f;

    StoredNetvars LagRecord;

    if (flOldSimTime[iEntityId] < flCurSimTime) {
        Store(&LagRecord, player);

        vecLagRecord[iEntityId].emplace_back(LagRecord);

        flOldSimTime[iEntityId] = flCurSimTime;
    }
}

void LagComp::RestorePlayer(C_BasePlayer *player) {
    if (!Settings::Resolver::LagComp)
        return;

    if (!engine->IsInGame())
        return;

    C_BasePlayer* me = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    if (!me || !player)
        return;

    if (vecLagRecord[player->GetIndex()].empty())
        return;

    StoredNetvars *src;

    pRecordRollback[player->GetIndex()] = src;

    Restore(src, player);
}

void Resolver::Hug( C_BasePlayer* player ) {
    if (!engine->IsInGame() || !player)
        return;

    auto cur = m_arrInfos.at( player->GetIndex() ).m_sRecords;
    float flYaw = 0;
    static float OldLowerBodyYaws[65];
    static float OldYawDeltas[65];
    float CurYaw = *player->GetLowerBodyYawTarget();
    static float oldTimer[65];
    static bool isLBYPredictited[65];
    INetChannelInfo* nci = engine->GetNetChannelInfo();
    float bodyeyedelta = player->GetEyeAngles()->y - cur.front().m_flLowerBodyYawTarget;
//-------------------NEW MEMES WOOOOH --------------------------------------------------------

    if ( OldLowerBodyYaws[player->GetIndex()] == *player->GetLowerBodyYawTarget() ) {
        if ( oldTimer[player->GetIndex()] + 1.1 >= globalVars->curtime ) {
            oldTimer[player->GetIndex()] = globalVars->curtime;
            isLBYPredictited[player->GetIndex()] = true;

        } else {
            isLBYPredictited[player->GetIndex()] = false;
        }
    } else if ( player->GetDormant() || !player->GetAlive() ) {
        oldTimer[player->GetIndex()] = -1;
        isLBYPredictited[player->GetIndex()] = false;
    } else {
        OldLowerBodyYaws[player->GetIndex()] = *player->GetLowerBodyYawTarget();
        oldTimer[player->GetIndex()] = globalVars->curtime - nci->GetAvgLatency( FLOW_OUTGOING ); // pValveC0denz
        isLBYPredictited[player->GetIndex()] = false;
    }



// END OF NEW MEMES WOOOOH ----------------------------------------------------------------------

    if ( Settings::Resolver::pitch ) {
        if ( player->GetEyeAngles()->x < -179.f ) player->GetEyeAngles()->x += 360.f;
        else if ( player->GetEyeAngles()->x > 90.0 || player->GetEyeAngles()->x < -90.0 )
            player->GetEyeAngles()->x = 89.f;
        else if ( player->GetEyeAngles()->x > 89.0 && player->GetEyeAngles()->x < 91.0 )
            player->GetEyeAngles()->x -= 90.f;
        else if ( player->GetEyeAngles()->x > 179.0 && player->GetEyeAngles()->x < 181.0 )
            player->GetEyeAngles()->x -= 180;
        else if ( player->GetEyeAngles()->x > -179.0 && player->GetEyeAngles()->x < -181.0 )
            player->GetEyeAngles()->x += 180;
        else if ( fabs( player->GetEyeAngles()->x ) == 0 )
            player->GetEyeAngles()->x = std::copysign( 89.0f, player->GetEyeAngles()->x );
    }

    switch ( Settings::Resolver::Hugtype ) {
       case ResolverHugtype::RASP: {
            if (HasStaticRealAngle(cur)) {
                player->GetEyeAngles()->y = player->GetPoseParameter()[0] * 360 - 180;
                }
             else if (HasSteadyDifference(cur)) {
                float tickdif = static_cast<float> (cur.front().tickcount - cur.at(1).tickcount);
                float lbydif = GetDelta(cur.front().m_flLowerBodyYawTarget, cur.at(1).m_flLowerBodyYawTarget);
                float ntickdif = static_cast<float> (globalVars->tickcount - cur.front().tickcount);
                int num4 = Shotsmissed % 6;
                switch (num4) {
                    case 0:
                        player->GetEyeAngles()->y -= 90.0f;
                        num4++;
                        break;
                    case 1:
                        player->GetEyeAngles()->y = 45.0f;
                        num4++;
                        break;
                    case 2:
                        player->GetEyeAngles()->y = -90.0f;
                        num4++;
                        break;
                    case 3:
                        player->GetEyeAngles()->y = 75.0f;
                        num4++;
                        break;
                    case 4:
                        player->GetEyeAngles()->y = -180.0f;
                        num4++;
                        break;
                    case 5:
                        player->GetEyeAngles()->y = -30.0f;
                        num4 = 0;
                        break;
            }} else if (LBYKeepsChanging(cur))
                player->GetEyeAngles()->y = GetLBYByComparingTicks(cur);

            else if (!(LowerBodyYawChanged(player))) {
                float fwyaw = 0;
                float LBY = (cur.front().m_flLowerBodyYawTarget);

                if (player->GetEyeAngles()->y == LBY || player->GetEyeAngles()->y == (LBY + 90) ||
                    player->GetEyeAngles()->y == (LBY - 90)) {
                    int numa = Shotsmissed % 4;
                    switch (numa)
                    {
                        case 0:
                            player->GetEyeAngles()->y = (cur.front().m_flLowerBodyYawTarget) - 15;
                            break;
                        case 1:
                            player->GetEyeAngles()->y += 40;
                            break;
                        case 2:
                            player->GetEyeAngles()->y += 15;
                            break;
                        case 3:
                            player->GetEyeAngles()->y -= 40;
                            break;
                    }

                }
            }
                else {
                    if ( player->GetVelocity().x  > 1 || player->GetVelocity().x < 1) {

                        player->GetEyeAngles()->y = (cur.front().m_flLowerBodyYawTarget);
                    }
                }
                break;

        }
        case ResolverHugtype::BRUTEHIV: {

            int missed = 0;
            int missed2 = 0;
            float fakeYaw;
            float fakeYaw2;
            float fakeYaw3;
            float fakeYaw4;


            if ( LowerBodyYawChanged( player ) ) {
                player->GetEyeAngles()->y = ( cur.front().m_flLowerBodyYawTarget + bodyeyedelta );

            } else {
                int num3 = Shotsmissed % 6;
                switch ( num3 ) {
                    case 0:
                        player->GetEyeAngles()->y -= 180.0f;
                        break;
                    case 1:
                        player->GetEyeAngles()->y += 90.0f;
                        break;
                    case 2:
                        player->GetEyeAngles()->y -= 90.0f;
                        break;
                    case 3:
                        player->GetEyeAngles()->y = 0.0f;
                        break;
                    case 4:
                        player->GetEyeAngles()->y = -180.0f;
                        break;
                    case 5:
                        player->GetEyeAngles()->y = -30.0f;
                        break;
                }


            }
            break;
        }
        case ResolverHugtype::BRUTE1: {
            int num2 = Shotsmissed % 6;
            switch ( num2 ) {
                case 0:
                    player->GetEyeAngles()->y -= 0.0f;
                    break;
                case 1:
                    player->GetEyeAngles()->y = 35.0f;
                    break;
                case 2:
                    player->GetEyeAngles()->y = -70.0f;
                    break;
                case 3:
                    player->GetEyeAngles()->y = 55.0f;
                    break;
                case 4:
                    player->GetEyeAngles()->y = -180.0f;
                    break;
                case 5:
                    player->GetEyeAngles()->y = -30.0f;
                    break;
            }
            break;
        }
        case ResolverHugtype::AUTISM: // dont use this atm
        {

            if ( HasStaticRealAngle( cur ) )
                player->GetEyeAngles()->y += 180;

            else if ( LowerBodyYawChanged( player ) )
                player->GetEyeAngles()->y = ( cur.front().m_flLowerBodyYawTarget + bodyeyedelta );
            else if ( HasStaticYawDifference( cur ) )
                player->GetEyeAngles()->y =
                        player->GetEyeAngles()->y -
                        ( cur.front().m_angEyeAngles.y - cur.front().m_flLowerBodyYawTarget );

            else if ( DeltaKeepsChanging( cur ) )
                player->GetEyeAngles()->y = player->GetEyeAngles()->y - GetDeltaByComparingTicks( cur );
            else if ( LBYKeepsChanging( cur ) )
                player->GetEyeAngles()->y = GetLBYByComparingTicks( cur );
            else
                player->GetEyeAngles()->y += ( cur.front().m_flLowerBodyYawTarget ) + 180;
            break;
        }
        case ResolverHugtype::MYRRIBDELTA: {
            // 1. Check if LBY is updated
            // 2. If yes -> LBY, If not -> 3.
            // 3. If Static angle -> If Miss on LBY -> brute
            // 4. Else Brute

            static bool isLBYupdated = false;
            static bool isMoving = false;
            static bool staticAngle = false;
            static bool firstLBYupdate = false;

            bool onGround = player->GetFlags() & FL_ONGROUND;
            float curTime = globalVars->curtime;
            float LBY = *player->GetLowerBodyYawTarget();

            static float nextUpdate = 0.f;

            static int shotsmissedSave = 0;
            static float shotsmissedTime = 0.f;

            if ( Shotsmissed > 0 && Shotsmissed != shotsmissedSave ) {
                shotsmissedTime = curTime + 4.f;
                shotsmissedSave = Shotsmissed;
            } else if ( shotsmissedTime > curTime && Shotsmissed == 0 ) {
                shotsmissedSave = 0;
            }

            if ( onGround && fabsf( player->GetVelocity().x ) > 1 ) {
                isMoving = true;
            } else {
                isMoving = false;
            }

            if ( firstLBYupdate && curTime > nextUpdate && onGround && fabsf( bodyeyedelta ) > 35.f ) {
                isLBYupdated = true;
            } else {
                isLBYupdated = false;
            }

            if ( HasStaticRealAngle( cur ) ) {
                staticAngle = true;
            } else {
                staticAngle = false;
            }

            if ( isMoving || isLBYupdated || fabsf( bodyeyedelta ) < 35.f ) {
                firstLBYupdate = true;
                nextUpdate = curTime + 1.1f;
                player->GetEyeAngles()->y = LBY;
            } else {
                // LBY is NOT updated
                if ( staticAngle ) {
                    if ( player->GetEyeAngles()->y == LBY ) {
                        int a = shotsmissedSave % 10;
                        switch ( a ) {
                            case 0:
                                player->GetEyeAngles()->y = LBY;
                                break;
                            case 1:
                                player->GetEyeAngles()->y = LBY + 180;
                                break;
                            case 2:
                                player->GetEyeAngles()->y = LBY + 90;
                                break;
                            case 3:
                                player->GetEyeAngles()->y = LBY + 135;
                                break;
                            case 4:
                                player->GetEyeAngles()->y = LBY - 90;
                                break;
                            case 5:
                                player->GetEyeAngles()->y = LBY + 45;
                                break;
                            case 6:
                                player->GetEyeAngles()->y = LBY - 45;
                                break;
                            case 7:
                                player->GetEyeAngles()->y = LBY - 135;
                                break;
                            case 8:
                                player->GetEyeAngles()->y = LBY + 100;
                                break;
                            case 9:
                                player->GetEyeAngles()->y = LBY - 100;
                                break;
                            default:
                                player->GetEyeAngles()->y = LBY;
                                break;
                        }
                    } else {
                        int b = shotsmissedSave % 10;
                        switch ( b ) {
                            case 0:
                                player->GetEyeAngles()->y = LBY;
                                break;
                            case 1:
                                player->GetEyeAngles()->y += 180;
                                break;
                            case 2:
                                player->GetEyeAngles()->y -= 90;
                                break;
                            case 3:
                                player->GetEyeAngles()->y += 135;
                                break;
                            case 4:
                                player->GetEyeAngles()->y += 90;
                                break;
                            case 5:
                                player->GetEyeAngles()->y -= 45;
                                break;
                            case 6:
                                player->GetEyeAngles()->y += 45;
                                break;
                            case 7:
                                player->GetEyeAngles()->y -= 135;
                                break;
                            case 8:
                                player->GetEyeAngles()->y -= 100;
                                break;
                            case 9:
                                player->GetEyeAngles()->y += 100;
                                break;
                            default:
                                player->GetEyeAngles()->y = LBY;
                                break;
                        }
                    }
                } else {
                    int c = shotsmissedSave % 11;
                    int rng = rand() % 180 + 1;
                    switch ( c ) {
                        case 0:
                            player->GetEyeAngles()->y = LBY;
                            break; // This should not even be used
                        case 1:
                            player->GetEyeAngles()->y += 180.f;
                            break;
                        case 2:
                            player->GetEyeAngles()->y = LBY + 180.f;
                            break; // This is where it will start
                        case 3:
                            player->GetEyeAngles()->y = LBY + 90.f;
                            break;
                        case 4:
                            player->GetEyeAngles()->y = LBY + 135.f;
                            break;
                        case 5:
                            player->GetEyeAngles()->y = LBY - 90.f;
                            break;
                        case 6:
                            player->GetEyeAngles()->y = LBY + 45.f;
                            break;
                        case 7:
                            player->GetEyeAngles()->y = LBY - 135.f;
                            break;
                        case 8:
                            player->GetEyeAngles()->y = LBY - 45.f;
                            break;
                        case 9:
                            player->GetEyeAngles()->y = LBY + rng;
                            break;
                        case 10:
                            player->GetEyeAngles()->y = LBY - rng;
                            break;
                        default:
                            player->GetEyeAngles()->y = LBY;
                            break;
                    }
                }
            }
        }
        case ResolverHugtype::MYRRIBDELTA2: {
            static bool lbyUpdatedM = false;
            static bool isMovingM = false;

            static float lbySaveM = 0;

            float curTime = globalVars->curtime;
            static float nextUpdate = 0;
            static bool lbyFirstUpdateM = false;

            float LBY = ( cur.front().m_flLowerBodyYawTarget );
            bool onGround = player->GetFlags() & FL_ONGROUND;

            if ( player->GetVelocity().Length2D() > 1 && onGround ) {
                isMovingM = true;
            } else {
                isMovingM = false;
            }

            if ( lbyFirstUpdateM && curTime > nextUpdate && fabsf( bodyeyedelta ) > 35.0f ) {
                lbyUpdatedM = true;
                nextUpdate = curTime + 1.1f;
            } else {
                lbyUpdatedM = false;
            }

            if ( lbyUpdatedM || isMovingM || fabsf( bodyeyedelta ) >= 35.0f ) {
                player->GetEyeAngles()->y = LBY;
                lbySaveM = LBY;
                lbyFirstUpdateM = true;
            } else if ( !onGround ) {
                player->GetEyeAngles()->y = lbySaveM;
            } else {
                if ( Shotsmissed > 2 ) {
                    int n = Shotsmissed % 9;

                    switch ( n ) {
                        case 0:
                            player->GetEyeAngles()->y = lbySaveM;
                            break;
                        case 1:
                            player->GetEyeAngles()->y = LBY + 180;
                            break;
                        case 2:
                            player->GetEyeAngles()->y = LBY + 90;
                            break;
                        case 3:
                            player->GetEyeAngles()->y = LBY - 90;
                            break;
                        case 4:
                            player->GetEyeAngles()->y = LBY + 135;
                            break;
                        case 5:
                            player->GetEyeAngles()->y = LBY - 135;
                            break;
                        case 6:
                            player->GetEyeAngles()->y = LBY + 45;
                            break;
                        case 7:
                            player->GetEyeAngles()->y = LBY - 45;
                            break;
                        case 8:
                            player->GetEyeAngles()->y = player->GetEyeAngles()->y + 180;
                            break;
                    }
                } else {
                    if ( fabsf( bodyeyedelta ) < 35.0f && fabsf( bodyeyedelta ) > 0.0f ) {
                        player->GetEyeAngles()->y = LBY + bodyeyedelta;
                    }
                }
            }
        }
        case ResolverHugtype::BACKTRACKLBY: {
            if (!Settings::Resolver::LagComp)
                Settings::Resolver::LagComp = true;

            static bool lbyUpdated = false;

            float curTime = globalVars->curtime;
            static float nextUpdate = 0;
            static bool lbyFirstUpdateM = false;

            float LBY = ( cur.front().m_flLowerBodyYawTarget );
            bool onGround = (player->GetFlags() & FL_ONGROUND);
            bool isMoving = (player->GetVelocity().Length2D() != 0 && onGround);
            bool staticAngle = (HasStaticRealAngle(cur));

            static int shotsmissedSave = 0;
            static float shotsmissedTime = 0.f;

            if ( Shotsmissed > 0 && Shotsmissed != shotsmissedSave ) {
                shotsmissedTime = curTime + 4.f;
                shotsmissedSave = Shotsmissed;
            } else if ( shotsmissedTime > curTime && Shotsmissed == 0 ) {
                shotsmissedSave = 0;
            }

            if ( curTime > nextUpdate || isMoving )
                lbyUpdated = true;
            else
                lbyUpdated = false;

            if ( lbyUpdated ) {
                player->GetEyeAngles()->y = LBY;
                nextUpdate = curTime + 1.1f;
            } else {
                if (Shotsmissed > 2) {
                    if ( staticAngle ) {
                        if ( player->GetEyeAngles()->y == LBY ) {
                            int a = shotsmissedSave % 10;
                            switch ( a ) {
                                case 0:
                                    player->GetEyeAngles()->y = LBY;
                                    break;
                                case 1:
                                    player->GetEyeAngles()->y = LBY + 180;
                                    break;
                                case 2:
                                    player->GetEyeAngles()->y = LBY + 90;
                                    break;
                                case 3:
                                    player->GetEyeAngles()->y = LBY + 135;
                                    break;
                                case 4:
                                    player->GetEyeAngles()->y = LBY - 90;
                                    break;
                                case 5:
                                    player->GetEyeAngles()->y = LBY + 45;
                                    break;
                                case 6:
                                    player->GetEyeAngles()->y = LBY - 45;
                                    break;
                                case 7:
                                    player->GetEyeAngles()->y = LBY - 135;
                                    break;
                                case 8:
                                    player->GetEyeAngles()->y = LBY + 100;
                                    break;
                                case 9:
                                    player->GetEyeAngles()->y = LBY - 100;
                                    break;
                                default:
                                    player->GetEyeAngles()->y = LBY;
                                    break;
                            }
                        } else {
                            int b = shotsmissedSave % 10;
                            switch ( b ) {
                                case 0:
                                    player->GetEyeAngles()->y = LBY;
                                    break;
                                case 1:
                                    player->GetEyeAngles()->y += 180;
                                    break;
                                case 2:
                                    player->GetEyeAngles()->y -= 90;
                                    break;
                                case 3:
                                    player->GetEyeAngles()->y += 135;
                                    break;
                                case 4:
                                    player->GetEyeAngles()->y += 90;
                                    break;
                                case 5:
                                    player->GetEyeAngles()->y -= 45;
                                    break;
                                case 6:
                                    player->GetEyeAngles()->y += 45;
                                    break;
                                case 7:
                                    player->GetEyeAngles()->y -= 135;
                                    break;
                                case 8:
                                    player->GetEyeAngles()->y -= 100;
                                    break;
                                case 9:
                                    player->GetEyeAngles()->y += 100;
                                    break;
                                default:
                                    player->GetEyeAngles()->y = LBY;
                                    break;
                            }
                        }
                    } else {
                        int c = shotsmissedSave % 11;
                        int rng = rand() % 180 + 1;
                        switch ( c ) {
                            case 0:
                                player->GetEyeAngles()->y = LBY;
                                break; // This should not even be used
                            case 1:
                                player->GetEyeAngles()->y += 180.f;
                                break;
                            case 2:
                                player->GetEyeAngles()->y = LBY + 180.f;
                                break; // This is where it will start
                            case 3:
                                player->GetEyeAngles()->y = LBY + 90.f;
                                break;
                            case 4:
                                player->GetEyeAngles()->y = LBY + 135.f;
                                break;
                            case 5:
                                player->GetEyeAngles()->y = LBY - 90.f;
                                break;
                            case 6:
                                player->GetEyeAngles()->y = LBY + 45.f;
                                break;
                            case 7:
                                player->GetEyeAngles()->y = LBY - 135.f;
                                break;
                            case 8:
                                player->GetEyeAngles()->y = LBY - 45.f;
                                break;
                            case 9:
                                player->GetEyeAngles()->y = LBY + rng;
                                break;
                            case 10:
                                player->GetEyeAngles()->y = LBY - rng;
                                break;
                            default:
                                player->GetEyeAngles()->y = LBY;
                                break;
                        }
                    }
                } else {
                }
            }
        }
        case ResolverHugtype::OFF:
            break;
    }
}

void Resolver::FrameStageNotify( ClientFrameStage_t stage ) {


    if ( !Settings::Resolver::enabled || !engine->IsInGame() )
        return;

    C_BasePlayer* me = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    if ( !me || !me->GetAlive() )
        return;

    if ( stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
        for ( int i = 1; i < engine->GetMaxClients(); ++i ) {
            C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

            if ( !target
                 || target == me
                 || target->GetDormant()
                 || !target->GetAlive()
                 || target->GetImmune()
                 || target->GetTeam() == me->GetTeam() )
                continue;

            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo( i, &entityInformation );

            if ( std::find( Resolver::playerAngleLogs.begin(), Resolver::playerAngleLogs.end(),
                            entityInformation.xuid ) == Resolver::playerAngleLogs.end() ) {
                player_data.push_back( std::pair<C_BasePlayer*, QAngle>( target, *target->GetEyeAngles() ) );
            }

            if ( shotATT ) {
                Shotsmissed++;
                shotATT = false;
            }

            float lby = *target->GetLowerBodyYawTarget();

            /* Lby will update every 0.022 seconds while moving (assume that is just all the time) */
            if ( target->GetVelocity().Length2D() > 0.1f && target->GetFlags() & FL_ONGROUND ) {
                /*
                float flCurTime = globalVars->curtime;
                static float flTimeUpdate = 0.5f;
                static float flNextTimeUpdate = flCurTime + flTimeUpdate;
                static bool bFlip = true;

                if (flCurTime >= flNextTimeUpdate) {
                    bFlip = !bFlip;
                }

                if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
                    flNextTimeUpdate = flCurTime + flTimeUpdate;

                if (bFlip) {
                    lby += 35.f;
                } else {
                    lby -= 35.f;
                }
                */
                target->GetEyeAngles()->y = lby;
                shotATT = true;
            } else {
                Hug( target );
                shotATT = true;
            }
        }
    } else if ( stage == ClientFrameStage_t::FRAME_RENDER_END ) {
        for ( unsigned long i = 0; i < player_data.size(); i++ ) {
            std::pair<C_BasePlayer*, QAngle> player_aa_data = player_data[i];
            *player_aa_data.first->GetEyeAngles() = player_aa_data.second;
        }

        player_data.clear();
    }
}

void Resolver::PostFrameStageNotify( ClientFrameStage_t stage ) {
}

CTickRecord Resolver::GetShotRecord( C_BasePlayer* player ) {
    for ( auto cur : m_arrInfos[player->GetIndex()].m_sRecords ) {
        if ( cur.validtick )
            return CTickRecord( cur );
    }
    return CTickRecord();
}

bool& Resolver::LowerBodyYawChanged( C_BasePlayer* player ) {
    return m_arrInfos.at( player->GetIndex() ).m_bLowerBodyYawChanged;
}

bool Resolver::HasStaticRealAngle( const std::deque<CTickRecord>& l, float tolerance ) {
    auto minmax = std::minmax_element( std::begin( l ), std::end( l ),
                                       []( const CTickRecord& t1, const CTickRecord& t2 ) {
                                           return t1.m_flLowerBodyYawTarget < t2.m_flLowerBodyYawTarget;
                                       } );
    return ( fabs( minmax.first->m_flLowerBodyYawTarget - minmax.second->m_flLowerBodyYawTarget ) <= tolerance );
}

bool Resolver::HasStaticRealAngle( int index, float tolerance ) {
    return HasStaticRealAngle( m_arrInfos[index].m_sRecords, tolerance );
}

bool Resolver::HasStaticYawDifference( const std::deque<CTickRecord>& l, float tolerance ) {
    for ( auto i = l.begin(); i < l.end() - 1; ) {
        if ( GetDelta( LBYDelta( *i ), LBYDelta( *++i ) ) > tolerance )
            return false;
    }
    return true;
}

bool Resolver::HasSteadyDifference( const std::deque<CTickRecord>& l, float tolerance ) {
    size_t misses = 0;
    for ( size_t i = 0; i < l.size() - 1; i++ ) {
        float tickdif = static_cast<float> (l.at( i ).m_flSimulationTime - l.at( i + 1 ).tickcount);
        float lbydif = GetDelta( l.at( i ).m_flLowerBodyYawTarget, l.at( i + 1 ).m_flLowerBodyYawTarget );
        float ntickdif = static_cast<float> (globalVars->tickcount - l.at( i ).tickcount);
        if ( ( ( lbydif / tickdif ) * ntickdif ) > tolerance ) misses++;
    }
    return ( misses <= ( l.size() / 3 ) );
}

int Resolver::GetDifferentDeltas( const std::deque<CTickRecord>& l, float tolerance ) {
    std::vector<float> vec;
    for ( auto var : l ) {
        float curdelta = LBYDelta( var );
        bool add = true;
        for ( auto fl : vec ) {
            if ( !IsDifferent( curdelta, fl, tolerance ) )
                add = false;
        }
        if ( add )
            vec.push_back( curdelta );
    }
    return vec.size();
}

int Resolver::GetDifferentLBYs( const std::deque<CTickRecord>& l, float tolerance ) {
    std::vector<float> vec;
    for ( auto var : l ) {
        float curyaw = var.m_flLowerBodyYawTarget;
        bool add = true;
        for ( auto fl : vec ) {
            if ( !IsDifferent( curyaw, fl, tolerance ) )
                add = false;
        }
        if ( add )
            vec.push_back( curyaw );
    }
    return vec.size();
}

bool Resolver::DeltaKeepsChanging( const std::deque<CTickRecord>& cur, float tolerance ) {
    return ( GetDifferentDeltas( cur ) > ( int ) cur.size() / 2 );
}

bool Resolver::LBYKeepsChanging( const std::deque<CTickRecord>& cur, float tolerance ) {
    return ( GetDifferentLBYs( cur, tolerance ) > ( int ) cur.size() / 2 );
}

float Resolver::GetLBYByComparingTicks( const std::deque<CTickRecord>& l ) {
    int modulo = Settings::Resolver::modulo;
    //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "\nmodulo " + modulo);
    int difangles = GetDifferentLBYs( l );
    int inc = modulo * difangles;
    for ( auto var : l ) {
        for ( int lasttick = var.tickcount; lasttick < globalVars->tickcount; lasttick += inc ) {
            if ( lasttick == globalVars->tickcount )
                return var.m_flLowerBodyYawTarget;
        }
    }
    return 0.f;
}

float Resolver::GetDeltaByComparingTicks( const std::deque<CTickRecord>& l ) {
    int modulo = Settings::Resolver::modulo;
    int difangles = GetDifferentDeltas( l );
    int inc = modulo * difangles;
    for ( auto var : l ) {
        for ( int lasttick = var.tickcount; lasttick < globalVars->tickcount; lasttick += inc ) {
            if ( lasttick == globalVars->tickcount )
                return LBYDelta( var );
        }
    }
    return 0.f;
}

void Resolver::FireGameEvent( IGameEvent* event ) {
    if ( !event )
        return;

    if ( strcmp( event->GetName(), XORSTR( "player_hurt" ) ) == 0 ) {
        int hurt_player_id = event->GetInt( XORSTR( "userid" ) );
        int attacker_id = event->GetInt( XORSTR( "attacker" ) );

        if ( engine->GetPlayerForUserID( hurt_player_id ) == engine->GetLocalPlayer() )
            return;

        if ( engine->GetPlayerForUserID( attacker_id ) != engine->GetLocalPlayer() )
            return;

        C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
        if ( !localplayer )
            return;

        C_BasePlayer* hurt_player = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( hurt_player_id ) );
        if ( !hurt_player )
            return;

        shotATT = false;

        IEngineClient::player_info_t localPlayerInfo;
        engine->GetPlayerInfo( localplayer->GetIndex(), &localPlayerInfo );

        IEngineClient::player_info_t hurtPlayerInfo;
        engine->GetPlayerInfo( hurt_player->GetIndex(), &hurtPlayerInfo );

        Shotsmissed = 0;
    }

    if ( strcmp( event->GetName(), "player_connect_full" ) != 0 &&
         strcmp( event->GetName(), "cs_game_disconnected" ) != 0 )
        return;

    if ( event->GetInt( "userid" ) &&
         engine->GetPlayerForUserID( event->GetInt( "userid" ) ) != engine->GetLocalPlayer() )
        return;

    Resolver::playerAngleLogs.clear();
}

void Resolver::CreateMove( CUserCmd* cmd ) {
    for ( int x = 1; x < engine->GetMaxClients(); ++x ) {
        C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( x );

        if ( !target
             || target == entityList->GetClientEntity( engine->GetLocalPlayer() )
             || target->GetDormant()
             || !target->GetAlive()
             || target->GetImmune()
             || target->GetTeam() == entityList->GetClientEntity( engine->GetLocalPlayer() )->GetTeam() )
            continue;
        if ( Settings::Resolver::LagComp )
            StartLagComp(target);
    }
}
