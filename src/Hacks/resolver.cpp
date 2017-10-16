#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::pitch = false;
float Settings::Resolver::ticks = 2;
float Settings::Resolver::modulo = 2;
bool Settings::Resolver::LagComp = false;
bool Settings::Resolver::lbyOnly = false;
std::vector<int64_t> Resolver::playerAngleLogs = {};
std::array<CResolveInfo, 32> Resolver::m_arrInfos;

ButtonCode_t Settings::Resolver::angleFlip = ButtonCode_t::KEY_F;
bool Settings::Resolver::angleFlipEnabled = false;

int Shotsmissed = 0;
bool shotATT;
std::vector<std::pair<C_BasePlayer*, QAngle>> player_data;
std::random_device rd;
static float rSimTime[65];

static int shotsMiss = 0;

bool didDmg = false;

bool Resolver::lbyUpdated = false;

bool Resolver::shouldBaim = false;

static void StartLagComp( C_BasePlayer* player, CUserCmd* cmd ) {
    if ( !Settings::Aimbot::backtrack )
        Settings::Aimbot::backtrack = true;
}

void Resolver::Hug( C_BasePlayer* player ) {
    QAngle angle = *player->GetEyeAngles();
    auto cur = m_arrInfos.at( player->GetIndex() ).m_sRecords;
    if (player->GetVelocity().Length2D() > 1)
        player->GetEyeAngles()->y = *player->GetLowerBodyYawTarget();
    else {
        if (HasStaticRealAngle(cur)) {
            int missed = Shotsmissed;
            switch (missed) {
                case 0:
                    player->GetEyeAngles()->y = *player->GetLowerBodyYawTarget();
                    break;
                case 1:
                    player->GetEyeAngles()->y += 45;
                    break;
                case 2:
                    player->GetEyeAngles()->y -= 45;
                    break;
                case 3:
                    player->GetEyeAngles()->y += 90;
                    break;
                case 4:
                    player->GetEyeAngles()->y -= 90;
                    break;
                case 5:
                    player->GetEyeAngles()->y += 110;
                    break;
                case 6:
                    player->GetEyeAngles()->y -= 110;
                    break;
                case 7:
                    player->GetEyeAngles()->y += 145;
                    break;
                case 8:
                    player->GetEyeAngles()->y -= 145;
                    break;
                case 9:
                    player->GetEyeAngles()->y -= 160;
                    break;
                case 10:
                    player->GetEyeAngles()->y -= 160;
                    break;
                case 11:
                    player->GetEyeAngles()->y -= 180;
                    break;
            }


        } else if (HasStaticYawDifference(cur))
            player->GetEyeAngles()->y -=
                    (cur.front().m_angEyeAngles.y - cur.front().m_flLowerBodyYawTarget);
        else if (HasSteadyDifference(cur)) {
            float tickdif = static_cast<float>(cur.front().tickcount - cur.at(1).tickcount);
            float lbydif = GetDelta(cur.front().m_flLowerBodyYawTarget, cur.at(1).m_flLowerBodyYawTarget);
            float ntickdif = static_cast<float>(globalVars->tickcount - cur.front().tickcount);
            player->GetEyeAngles()->y = (lbydif / tickdif) * ntickdif;
        } else if (DeltaKeepsChanging(cur))
            player->GetEyeAngles()->y = GetDeltaByComparingTicks(cur);
        else if (LBYKeepsChanging(cur))
            player->GetEyeAngles()->y = GetLBYByComparingTicks(cur);
    }
        if ( Settings::Resolver::pitch ) {
            if ( angle.x < -179.f ) angle.x += 360.f;
            else if ( angle.x > 90.0 || angle.x < -90.0 )
                angle.x = 89.f;
            else if ( angle.x > 89.0 && angle.x < 91.0 )
                angle.x -= 90.f;
            else if ( angle.x > 179.0 && angle.x < 181.0 )
                angle.x -= 180;
            else if ( angle.x > -179.0 && angle.x < -181.0 )
                angle.x += 180;
            else if ( fabs( angle.x ) == 0 )
                angle.x = std::copysign( 89.0f, angle.x );
        }

        player->GetEyeAngles()->y = player->GetEyeAngles()->y;
        player->GetEyeAngles()->x = angle.x;
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

            Hug ( target );
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

void Resolver::StoreVars( C_BasePlayer* player ) {
    if ( m_arrInfos.at( player->GetIndex() ).m_sRecords.size() >= Settings::Resolver::ticks ) {
        m_arrInfos.at( player->GetIndex() ).m_sRecords.pop_back();
    }
    m_arrInfos.at( player->GetIndex() ).m_sRecords.push_front( CTickRecord( player ) );
}

void Resolver::StoreVars( C_BasePlayer* player, QAngle ang, float lby, float simtime, float tick ) {
    if ( m_arrInfos.at( player->GetIndex() ).m_sRecords.size() >= Settings::Resolver::ticks )
        m_arrInfos.at( player->GetIndex() ).m_sRecords.pop_back();
    m_arrInfos.at( player->GetIndex() ).m_sRecords.push_front( CTickRecord( player ) );
}

/*bool& Resolver::BacktrackThisTick( C_BasePlayer* player ) {
    return m_arrInfos.at( player->GetIndex() ).m_bBacktrackThisTick;
}*/

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

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );                

    if ( !localplayer || !localplayer->GetAlive() )
        return;

    if ( strcmp( event->GetName(), XORSTR( "player_hurt" ) ) == 0 ) {
        int hurt_player_id = event->GetInt( XORSTR( "userid" ) );
        int attacker_id = event->GetInt( XORSTR( "attacker" ) );

        if ( engine->GetPlayerForUserID( hurt_player_id ) == engine->GetLocalPlayer() )
            return;

        if ( engine->GetPlayerForUserID( attacker_id ) != engine->GetLocalPlayer() )
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

        didDmg = true;

        Shotsmissed = 0;

        shotsMiss = 0;
    } else {
        C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon() );
        
        int ammo = activeWeapon->GetAmmo();        
        static int ammoSave = ammo;

        if ( ammoSave != ammo ) {
            shotsMiss++;
            ammoSave = ammo;
        }
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