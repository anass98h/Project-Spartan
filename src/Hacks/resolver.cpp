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


static void StartLagComp( C_BasePlayer* player, CUserCmd* cmd ) {
    ConVar* cvar_cl_interp = cvar->FindVar( XORSTR( "cl_interp" ) );
    ConVar* cvar_cl_updaterate = cvar->FindVar( XORSTR( "cl_updaterate" ) );
    ConVar* cvar_cl_interp_ratio = cvar->FindVar( XORSTR( "cl_interp_ratio" ) );
    // is this lagcomp ? :/
    float cl_interp = cvar_cl_interp->fValue;
    float cl_updaterate = cvar_cl_updaterate->fValue;
    float cl_interp_ratio = cvar_cl_interp_ratio->fValue;
    float tick = ( player->GetSimulationTime() + std::max( cl_interp, cl_interp_ratio / cl_updaterate ) );
    cmd->tick_count = TIME_TO_TICKS( tick );
}

void Resolver::Hug( C_BasePlayer* Circlebian ) {
    auto cur = m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords;
    float flYaw = 0;
    static float OldLowerBodyYaws[65];
    static float OldYawDeltas[65];
    float CurYaw = *Circlebian->GetLowerBodyYawTarget();
    static float oldTimer[65];
    static bool isLBYPredictited[65];
    INetChannelInfo* nci = engine->GetNetChannelInfo();
    float bodyeyedelta = Circlebian->GetEyeAngles()->y - cur.front().m_flLowerBodyYawTarget;
//-------------------NEW MEMES WOOOOH --------------------------------------------------------

    if ( OldLowerBodyYaws[Circlebian->GetIndex()] == *Circlebian->GetLowerBodyYawTarget() ) {
        if ( oldTimer[Circlebian->GetIndex()] + 1.1 >= globalVars->curtime ) {
            oldTimer[Circlebian->GetIndex()] = globalVars->curtime;
            isLBYPredictited[Circlebian->GetIndex()] = true;

        } else {
            isLBYPredictited[Circlebian->GetIndex()] = false;
        }
    } else if ( Circlebian->GetDormant() || !Circlebian->GetAlive() ) {
        oldTimer[Circlebian->GetIndex()] = -1;
        isLBYPredictited[Circlebian->GetIndex()] = false;
    } else {
        OldLowerBodyYaws[Circlebian->GetIndex()] = *Circlebian->GetLowerBodyYawTarget();
        oldTimer[Circlebian->GetIndex()] = globalVars->curtime - nci->GetAvgLatency( FLOW_OUTGOING ); // pValveC0denz
        isLBYPredictited[Circlebian->GetIndex()] = false;
    }



// END OF NEW MEMES WOOOOH ----------------------------------------------------------------------

    if ( Settings::Resolver::pitch ) {
        if ( Circlebian->GetEyeAngles()->x < -179.f ) Circlebian->GetEyeAngles()->x += 360.f;
        else if ( Circlebian->GetEyeAngles()->x > 90.0 || Circlebian->GetEyeAngles()->x < -90.0 )
            Circlebian->GetEyeAngles()->x = 89.f;
        else if ( Circlebian->GetEyeAngles()->x > 89.0 && Circlebian->GetEyeAngles()->x < 91.0 )
            Circlebian->GetEyeAngles()->x -= 90.f;
        else if ( Circlebian->GetEyeAngles()->x > 179.0 && Circlebian->GetEyeAngles()->x < 181.0 )
            Circlebian->GetEyeAngles()->x -= 180;
        else if ( Circlebian->GetEyeAngles()->x > -179.0 && Circlebian->GetEyeAngles()->x < -181.0 )
            Circlebian->GetEyeAngles()->x += 180;
        else if ( fabs( Circlebian->GetEyeAngles()->x ) == 0 )
            Circlebian->GetEyeAngles()->x = std::copysign( 89.0f, Circlebian->GetEyeAngles()->x );
    }

    switch ( Settings::Resolver::Hugtype ) {
        case ResolverHugtype::RASP: {
            if (HasStaticRealAngle(cur)) {
                int num4 = Shotsmissed % 6;
                switch (num4) {
                    case 0:
                        Circlebian->GetEyeAngles()->y -= 90.0f;
                        num4++;
                        break;
                    case 1:
                        Circlebian->GetEyeAngles()->y = 45.0f;
                        num4++;
                        break;
                    case 2:
                        Circlebian->GetEyeAngles()->y = -90.0f;
                        num4++;
                        break;
                    case 3:
                        Circlebian->GetEyeAngles()->y = 75.0f;
                        num4++;
                        break;
                    case 4:
                        Circlebian->GetEyeAngles()->y = -180.0f;
                        num4++;
                        break;
                    case 5:
                        Circlebian->GetEyeAngles()->y = -30.0f;
                        num4 = 0;
                        break;
                }
            } else if (HasSteadyDifference(cur)) {
                float tickdif = static_cast<float> (cur.front().tickcount - cur.at(1).tickcount);
                float lbydif = GetDelta(cur.front().m_flLowerBodyYawTarget, cur.at(1).m_flLowerBodyYawTarget);
                float ntickdif = static_cast<float> (globalVars->tickcount - cur.front().tickcount);
                Circlebian->GetEyeAngles()->y = Circlebian->GetPoseParameter() * 360 - 180;
            } else if (LBYKeepsChanging(cur))
                Circlebian->GetEyeAngles()->y = GetLBYByComparingTicks(cur);

            else if (!LowerBodyYawChanged(Circlebian)) {
                float fwyaw = 0;
                float LBY = (cur.front().m_flLowerBodyYawTarget);

                if (Circlebian->GetEyeAngles()->y == LBY || Circlebian->GetEyeAngles()->y == LBY + 90 ||
                    Circlebian->GetEyeAngles()->y == LBY - 90) {
                    Circlebian->GetEyeAngles()->y = Circlebian->GetPoseParameter() * 360 - 180;

                }
            }
                else {
                    if ( Circlebian->GetVelocity().x  > 1 || Circlebian->GetVelocity().x < 1) {

                        Circlebian->GetEyeAngles()->y = (cur.front().m_flLowerBodyYawTarget);
                    }
                }
                break;

        }
        case ResolverHugtype::PASTEHUB: {

            if ( HasStaticRealAngle( cur ) ) {
                int num4 = Shotsmissed % 6;
                switch ( num4 ) {
                    case 0:
                        Circlebian->GetEyeAngles()->y -= 0.0f;
                        break;
                    case 1:
                        Circlebian->GetEyeAngles()->y = 35.0f;
                        break;
                    case 2:
                        Circlebian->GetEyeAngles()->y = -70.0f;
                        break;
                    case 3:
                        Circlebian->GetEyeAngles()->y = 55.0f;
                        break;
                    case 4:
                        Circlebian->GetEyeAngles()->y = -180.0f;
                        break;
                    case 5:
                        Circlebian->GetEyeAngles()->y = -30.0f;
                        break;
                }
            } else if ( HasStaticYawDifference( cur ) )
                Circlebian->GetEyeAngles()->y =
                        Circlebian->GetEyeAngles()->y -
                        ( cur.front().m_angEyeAngles.y - cur.front().m_flLowerBodyYawTarget );
            else if ( HasSteadyDifference( cur ) ) {
                float tickdif = static_cast<float> (cur.front().tickcount - cur.at( 1 ).tickcount);
                float lbydif = GetDelta( cur.front().m_flLowerBodyYawTarget, cur.at( 1 ).m_flLowerBodyYawTarget );
                float ntickdif = static_cast<float> (globalVars->tickcount - cur.front().tickcount);
                Circlebian->GetEyeAngles()->y = ( lbydif / tickdif ) * ntickdif;
            } else if ( DeltaKeepsChanging( cur ) )
                Circlebian->GetEyeAngles()->y = Circlebian->GetEyeAngles()->y - GetDeltaByComparingTicks( cur );
            else if ( LBYKeepsChanging( cur ) )
                Circlebian->GetEyeAngles()->y = GetLBYByComparingTicks( cur );

            else if ( !LowerBodyYawChanged( Circlebian ) ) {
                float fwyaw = 0;
                if ( Shotsmissed == 0 ) {
                    fwyaw -= 180;
                } else if ( Shotsmissed == 1 )
                    fwyaw += 90;

                else if ( Shotsmissed == 2 )
                    fwyaw += 180;

                else if ( Shotsmissed == 3 )
                    fwyaw -= 45;

                else if ( Shotsmissed == 4 )
                    fwyaw -= 90;

                else if ( Shotsmissed == 5 )
                    fwyaw -= 30;

                else if ( Shotsmissed == 6 )
                    fwyaw += 150;

                else {
                    fwyaw += bodyeyedelta;
                }

                Circlebian->GetEyeAngles()->y = fwyaw;
            } else {
                if ( LowerBodyYawChanged( Circlebian ) ) {
                    Circlebian->GetEyeAngles()->y = ( cur.front().m_flLowerBodyYawTarget);
                } else if ( Shotsmissed == 3 ) {
                    flYaw = flYaw - 180;
                    Circlebian->GetEyeAngles()->y = flYaw;
                } else if ( Shotsmissed == 4 ) {
                    flYaw = flYaw + 90;
                    Circlebian->GetEyeAngles()->y = flYaw;
                } else if ( Shotsmissed == 5 && Shotsmissed < 6 ) {
                    flYaw = flYaw + 180;
                    Circlebian->GetEyeAngles()->y = flYaw;
                } else if ( OldLowerBodyYaws[Circlebian->GetIndex()] == CurYaw ) {


                    Circlebian->GetEyeAngles()->y -= OldYawDeltas[Circlebian->GetIndex()];

                } else if ( Shotsmissed > 3 && isLBYPredictited[Circlebian->GetIndex()] == true &&
                            LowerBodyYawChanged( Circlebian ) ) {
                    Circlebian->GetEyeAngles()->y = *Circlebian->GetLowerBodyYawTarget();
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


            if ( LowerBodyYawChanged( Circlebian ) ) {
                Circlebian->GetEyeAngles()->y = ( cur.front().m_flLowerBodyYawTarget + bodyeyedelta );

            } else {
                int num3 = Shotsmissed % 6;
                switch ( num3 ) {
                    case 0:
                        Circlebian->GetEyeAngles()->y -= 180.0f;
                        break;
                    case 1:
                        Circlebian->GetEyeAngles()->y += 90.0f;
                        break;
                    case 2:
                        Circlebian->GetEyeAngles()->y -= 90.0f;
                        break;
                    case 3:
                        Circlebian->GetEyeAngles()->y = 0.0f;
                        break;
                    case 4:
                        Circlebian->GetEyeAngles()->y = -180.0f;
                        break;
                    case 5:
                        Circlebian->GetEyeAngles()->y = -30.0f;
                        break;
                }


            }
            break;
        }
        case ResolverHugtype::BRUTE1: {
            int num2 = Shotsmissed % 6;
            switch ( num2 ) {
                case 0:
                    Circlebian->GetEyeAngles()->y -= 0.0f;
                    break;
                case 1:
                    Circlebian->GetEyeAngles()->y = 35.0f;
                    break;
                case 2:
                    Circlebian->GetEyeAngles()->y = -70.0f;
                    break;
                case 3:
                    Circlebian->GetEyeAngles()->y = 55.0f;
                    break;
                case 4:
                    Circlebian->GetEyeAngles()->y = -180.0f;
                    break;
                case 5:
                    Circlebian->GetEyeAngles()->y = -30.0f;
                    break;
            }
            break;
        }
        case ResolverHugtype::AUTISM: // dont use this atm
        {

            if ( HasStaticRealAngle( cur ) )
                Circlebian->GetEyeAngles()->y += 180;

            else if ( LowerBodyYawChanged( Circlebian ) )
                Circlebian->GetEyeAngles()->y = ( cur.front().m_flLowerBodyYawTarget + bodyeyedelta );
            else if ( HasStaticYawDifference( cur ) )
                Circlebian->GetEyeAngles()->y =
                        Circlebian->GetEyeAngles()->y -
                        ( cur.front().m_angEyeAngles.y - cur.front().m_flLowerBodyYawTarget );

            else if ( DeltaKeepsChanging( cur ) )
                Circlebian->GetEyeAngles()->y = Circlebian->GetEyeAngles()->y - GetDeltaByComparingTicks( cur );
            else if ( LBYKeepsChanging( cur ) )
                Circlebian->GetEyeAngles()->y = GetLBYByComparingTicks( cur );
            else
                Circlebian->GetEyeAngles()->y += ( cur.front().m_flLowerBodyYawTarget ) + 180;
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

            bool onGround = Circlebian->GetFlags() & FL_ONGROUND;
            float curTime = globalVars->curtime;
            float LBY = *Circlebian->GetLowerBodyYawTarget();

            static float nextUpdate = 0.f;

            static int shotsmissedSave = 0;
            static float shotsmissedTime = 0.f;

            if ( Shotsmissed > 0 && Shotsmissed != shotsmissedSave ) {
                shotsmissedTime = curTime + 4.f;
                shotsmissedSave = Shotsmissed;
            } else if ( shotsmissedTime > curTime && Shotsmissed == 0 ) {
                shotsmissedSave = 0;
            }

            if ( onGround && fabsf( Circlebian->GetVelocity().x ) > 1 ) {
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
                Circlebian->GetEyeAngles()->y = LBY;
            } else {
                // LBY is NOT updated
                if ( staticAngle ) {
                    if ( Circlebian->GetEyeAngles()->y == LBY ) {
                        int a = shotsmissedSave % 10;
                        switch ( a ) {
                            case 0:
                                Circlebian->GetEyeAngles()->y = LBY;
                                break;
                            case 1:
                                Circlebian->GetEyeAngles()->y = LBY + 180;
                                break;
                            case 2:
                                Circlebian->GetEyeAngles()->y = LBY + 90;
                                break;
                            case 3:
                                Circlebian->GetEyeAngles()->y = LBY + 135;
                                break;
                            case 4:
                                Circlebian->GetEyeAngles()->y = LBY - 90;
                                break;
                            case 5:
                                Circlebian->GetEyeAngles()->y = LBY + 45;
                                break;
                            case 6:
                                Circlebian->GetEyeAngles()->y = LBY - 45;
                                break;
                            case 7:
                                Circlebian->GetEyeAngles()->y = LBY - 135;
                                break;
                            case 8:
                                Circlebian->GetEyeAngles()->y = LBY + 100;
                                break;
                            case 9:
                                Circlebian->GetEyeAngles()->y = LBY - 100;
                                break;
                            default:
                                Circlebian->GetEyeAngles()->y = LBY;
                                break;
                        }
                    } else {
                        int b = shotsmissedSave % 10;
                        switch ( b ) {
                            case 0:
                                Circlebian->GetEyeAngles()->y = LBY;
                                break;
                            case 1:
                                Circlebian->GetEyeAngles()->y += 180;
                                break;
                            case 2:
                                Circlebian->GetEyeAngles()->y -= 90;
                                break;
                            case 3:
                                Circlebian->GetEyeAngles()->y += 135;
                                break;
                            case 4:
                                Circlebian->GetEyeAngles()->y += 90;
                                break;
                            case 5:
                                Circlebian->GetEyeAngles()->y -= 45;
                                break;
                            case 6:
                                Circlebian->GetEyeAngles()->y += 45;
                                break;
                            case 7:
                                Circlebian->GetEyeAngles()->y -= 135;
                                break;
                            case 8:
                                Circlebian->GetEyeAngles()->y -= 100;
                                break;
                            case 9:
                                Circlebian->GetEyeAngles()->y += 100;
                                break;
                            default:
                                Circlebian->GetEyeAngles()->y = LBY;
                                break;
                        }
                    }
                } else {
                    int c = shotsmissedSave % 11;
                    int rng = rand() % 180 + 1;
                    switch ( c ) {
                        case 0:
                            Circlebian->GetEyeAngles()->y = LBY;
                            break; // This should not even be used
                        case 1:
                            Circlebian->GetEyeAngles()->y += 180.f;
                            break;
                        case 2:
                            Circlebian->GetEyeAngles()->y = LBY + 180.f;
                            break; // This is where it will start
                        case 3:
                            Circlebian->GetEyeAngles()->y = LBY + 90.f;
                            break;
                        case 4:
                            Circlebian->GetEyeAngles()->y = LBY + 135.f;
                            break;
                        case 5:
                            Circlebian->GetEyeAngles()->y = LBY - 90.f;
                            break;
                        case 6:
                            Circlebian->GetEyeAngles()->y = LBY + 45.f;
                            break;
                        case 7:
                            Circlebian->GetEyeAngles()->y = LBY - 135.f;
                            break;
                        case 8:
                            Circlebian->GetEyeAngles()->y = LBY - 45.f;
                            break;
                        case 9:
                            Circlebian->GetEyeAngles()->y = LBY + rng;
                            break;
                        case 10:
                            Circlebian->GetEyeAngles()->y = LBY - rng;
                            break;
                        default:
                            Circlebian->GetEyeAngles()->y = LBY;
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
            bool onGround = Circlebian->GetFlags() & FL_ONGROUND;

            if ( Circlebian->GetVelocity().Length2D() > 1 && onGround ) {
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
                Circlebian->GetEyeAngles()->y = LBY;
                lbySaveM = LBY;
                lbyFirstUpdateM = true;
            } else if ( !onGround ) {
                Circlebian->GetEyeAngles()->y = lbySaveM;
            } else {
                if ( Shotsmissed > 2 ) {
                    int n = Shotsmissed % 9;

                    switch ( n ) {
                        case 0:
                            Circlebian->GetEyeAngles()->y = lbySaveM;
                            break;
                        case 1:
                            Circlebian->GetEyeAngles()->y = LBY + 180;
                            break;
                        case 2:
                            Circlebian->GetEyeAngles()->y = LBY + 90;
                            break;
                        case 3:
                            Circlebian->GetEyeAngles()->y = LBY - 90;
                            break;
                        case 4:
                            Circlebian->GetEyeAngles()->y = LBY + 135;
                            break;
                        case 5:
                            Circlebian->GetEyeAngles()->y = LBY - 135;
                            break;
                        case 6:
                            Circlebian->GetEyeAngles()->y = LBY + 45;
                            break;
                        case 7:
                            Circlebian->GetEyeAngles()->y = LBY - 45;
                            break;
                        case 8:
                            Circlebian->GetEyeAngles()->y = Circlebian->GetEyeAngles()->y + 180;
                            break;
                    }
                } else {
                    if ( fabsf( bodyeyedelta ) < 35.0f && fabsf( bodyeyedelta ) > 0.0f ) {
                        Circlebian->GetEyeAngles()->y = LBY + bodyeyedelta;
                    }
                }
            }
        }
        case ResolverHugtype::POSEPARAMMEME: {
            static bool lbyUpdated = false;

            float curTime = globalVars->curtime;
            bool onGround = (Circlebian->GetFlags() & FL_ONGROUND);
            bool isMoving = (Circlebian->GetVelocity().Length2D() > 1);
            static float nextUpdate;

            if (onGround && curTime > nextUpdate || onGround && isMoving)
                lbyUpdated = true;
            else
                lbyUpdated = false;

            if (lbyUpdated) {
                Circlebian->GetEyeAngles()->y = *Circlebian->GetLowerBodyYawTarget();
                nextUpdate = curTime + 1.1f;
            } else {
                Circlebian->GetEyeAngles()->y = Circlebian->GetPoseParameter() * 360 - 180;
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

CTickRecord Resolver::GetShotRecord( C_BasePlayer* Circlebian ) {
    for ( auto cur : m_arrInfos[Circlebian->GetIndex()].m_sRecords ) {
        if ( cur.validtick )
            return CTickRecord( cur );
    }
    return CTickRecord();
}

bool& Resolver::LowerBodyYawChanged( C_BasePlayer* Circlebian ) {
    return m_arrInfos.at( Circlebian->GetIndex() ).m_bLowerBodyYawChanged;
}

void Resolver::StoreVars( C_BasePlayer* Circlebian ) {
    if ( m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords.size() >= Settings::Resolver::ticks ) {
        m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords.pop_back();
    }
    m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords.push_front( CTickRecord( Circlebian ) );
}

void Resolver::StoreVars( C_BasePlayer* Circlebian, QAngle ang, float lby, float simtime, float tick ) {
    if ( m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords.size() >= Settings::Resolver::ticks )
        m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords.pop_back();
    m_arrInfos.at( Circlebian->GetIndex() ).m_sRecords.push_front( CTickRecord( Circlebian ) );
}

bool& Resolver::BacktrackThisTick( C_BasePlayer* Circlebian ) {
    return m_arrInfos.at( Circlebian->GetIndex() ).m_bBacktrackThisTick;
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

        Resolver::StoreVars( target );

        if ( Settings::Resolver::LagComp )
            StartLagComp( target, cmd );

    }
}
