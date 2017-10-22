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
bool didDmg = false;

// extern
bool Resolver::lbyUpdated = false;
int Resolver::resolvingId = -1;
std::map<int, int> shotsMissed = {
    { -1, 0 }
};
std::map<int, int> shotsMissedSave = {
    { -1, 0 }
};

void Resolver::Hug( C_BasePlayer* target ) {
    if ( Settings::Resolver::resolvePitch )
        HugPitch( target );
    
    QAngle angle = *target->GetEyeAngles();

    Resolver::resolvingId = target->GetIndex();

    float velocity = target->GetVelocity().Length2D();
    bool onGround = target->GetFlags() & FL_ONGROUND;
    bool isMoving = onGround && velocity > 35.f;
    float serverTime = target->GetTickBase() * globalVars->interval_per_tick;
    float curTime = globalVars->curtime;
    float lastLbyUpdate = 0;
    float lby = *target->GetLowerBodyYawTarget();
    float shotsMissedTime = 2.f;
    float lastShotsMissed = 0;

    static int shotsMissedS = Resolver::shotsMissed[target->GetIndex()];

    if ( Resolver::shotsMissed[target->GetIndex()] > shotsMissedS ) {
        shotsMissedS = Resolver::shotsMissed[target->GetIndex()];
        Resolver::shotsMissedSave[target->GetIndex()] = Resolver::shotsMissed[target->GetIndex()];
       // lastShotsMissed = curTime; Never used
    } else if ( curTime > lastShotsMissed + shotsMissedTime ) {
        shotsMissedS = Resolver::shotsMissed[target->GetIndex()];
        Resolver::shotsMissedSave[target->GetIndex()] = Resolver::shotsMissed[target->GetIndex()];
    }

    Resolver::lbyUpdated = isMoving || serverTime == lastLbyUpdate + 1.1f;

    if ( Resolver::lbyUpdated )
        lastLbyUpdate = serverTime;

    if ( Resolver::lbyUpdated || Backtracking::backtrackingLby )
        angle.y = lby;
    else {
        // Call the pCode here
        // Call HugBrute(), etc. so we have clean code and not messy like before
        // For example
        // angle.y = HugLby( target );
        // TODO: Maybe add baim code here until we have something proper
       angle.y = Resolver::HugBrute(target);
    }

    if ( didDmg ) {
        // You can save angles here, etc...

        didDmg = false;
    }
    if(!onGround)
        Resolver::baimNextShot;
    target->GetEyeAngles()->y = angle.y;
}

float Resolver::HugLby( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();
    float lby = *target->GetLowerBodyYawTarget();
    studiohdr_t* hdr = modelInfo->GetStudioModel( target->GetModel() );

    if ( hdr && hdr->pSeqdesc( target->GetSequence() )->activity == 979 ) { // Let's use 979 instead of ACT_CSGO_IDLE_TURN_BALANCEADJUST, just to be sure
        // LBY broken and is between LBY+120 & LBY-120
        // Giving us range of 60 + 60 = 120
        // We should force 180 first, cuz its the most common one AFAIK
        angle.y = lby + 180;
    } else {
       angle.y = lby; 
    }

    return angle.y;
}

float Resolver::HugBrute( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();
    
    float lby = *target->GetLowerBodyYawTarget();

    switch ( Resolver::shotsMissedSave[target->GetIndex()] % 4 ) {
        case 0: angle.y = HugLby(target); break;
        case 1: angle.y = lby + 90.f; break;
        case 2: angle.y = lby - 90.f; break;
        case 3: angle.y = lby + 180.f; break;

    }
    return angle.y;
}

void Resolver::HugPitch( C_BasePlayer* target ) {
    if ( !Settings::Resolver::resolvePitch )
        return;

    QAngle angle = *target->GetEyeAngles();

    target->GetEyeAngles()->x = angle.x;
}

////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\
///                   HOOKS                   \\\
////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\

void Resolver::FireGameEvent( IGameEvent* event ) {
    if ( !event )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );                

    if ( !pLocal || !pLocal->GetAlive() )
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

        IEngineClient::player_info_t localPlayerInfo;
        engine->GetPlayerInfo( pLocal->GetIndex(), &localPlayerInfo );

        IEngineClient::player_info_t hurtPlayerInfo;
        engine->GetPlayerInfo( hurt_player->GetIndex(), &hurtPlayerInfo );

        didDmg = true;

        Resolver::shotsMissed[Resolver::resolvingId] = 0;
    } else {
        C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            pLocal->GetActiveWeapon() );
        
        int ammo = activeWeapon->GetAmmo();        
        static int ammoSave = ammo;

        if ( ammoSave != ammo ) {
            Resolver::shotsMissed[Resolver::resolvingId]++;
            ammoSave = ammo;
        }
    }

    if ( strcmp( event->GetName(), "player_connect_full" ) != 0 &&
         strcmp( event->GetName(), "cs_game_disconnected" ) != 0 )
        return;

    if ( event->GetInt( "userid" ) &&
         engine->GetPlayerForUserID( event->GetInt( "userid" ) ) != engine->GetLocalPlayer() )
        return;
}

void Resolver::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !Settings::Resolver::enabled || !engine->IsInGame() ) {
        return;
    }

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    // Idea: Add Resolve When Dead feature?
    if ( !pLocal || !pLocal->GetAlive() ) {
        return;
    }

    switch ( stage ) {
        case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START:
            // If we should use Aimbot target
            if ( Aimbot::useAbTarget ) {
                C_BasePlayer* abTarget = ( C_BasePlayer* ) entityList->GetClientEntity( Aimbot::targetAimbot );
                Hug ( abTarget );
            } else {
                // If not, loop trough the entity list
                for ( int i = 1; i < engine->GetMaxClients(); i++ ) {
                    C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );
        
                    if ( !target
                         || target == pLocal
                         || target->GetDormant()
                         || !target->GetAlive()
                         || target->GetImmune()
                         || target->GetTeam() == pLocal->GetTeam() )
                        continue;
        
                    Hug ( target );
                }
            }
            break;
        case ClientFrameStage_t::FRAME_RENDER_END:
            // Nothing in here needed, I guess
            break;
    }
}

void Resolver::PostFrameStageNotify( ClientFrameStage_t stage ) {
    // Nothing in here needed, I guess
}

void Resolver::CreateMove( CUserCmd* cmd ) {
    // I guess nothing in here too
}

void Resolver::Paint() {
    // I guess we dont need to draw anything, because we have Resolver Info Window
    // We should draw everything there!
}
