#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::resolvePitch = false;
bool Settings::Resolver::lagCompensation = false;
bool Settings::Resolver::headshotLbyUpdateOnly = false;

bool Settings::SmartAim::enabled = false;
int Settings::SmartAim::baimAfterMissed = 3;

bool Settings::AngleFlip::enabled = false;
ButtonCode_t Settings::AngleFlip::key = ButtonCode_t::KEY_F;

// Local variables
bool didDmg = false;
std::map<int, int> shotsMissed = { { -1, 0 } };
std::map<int, int> shotsMissedSave = { { -1, 0 } };
static int lastAmmo = -1;

// Global variables
bool Resolver::lbyUpdated = false;
int Resolver::resolvingId = -1;

void Resolver::Hug( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    Resolver::resolvingId = target->GetIndex();

    float velocity = target->GetVelocity().Length2D();
    bool onGround = target->GetFlags() & FL_ONGROUND;
    bool isMoving = onGround && velocity > 35.f;

    float serverTime = target->GetTickBase() * globalVars->interval_per_tick;
    float curTime = globalVars->curtime;
    float lastLbyUpdate = 0.0f;

    float shotsMissedTime = 2.0f;
    float lastShotsMissed = 0.0f;

    static int shotsMissedS = Resolver::shotsMissed[target->GetIndex()];

    if ( Resolver::shotsMissed[target->GetIndex()] > shotsMissedS ) {
        shotsMissedS = Resolver::shotsMissed[target->GetIndex()];
        Resolver::shotsMissedSave[target->GetIndex()] = Resolver::shotsMissed[target->GetIndex()];
        lastShotsMissed = curTime;
    } else if ( curTime > lastShotsMissed + shotsMissedTime ) {
        shotsMissedS = Resolver::shotsMissed[target->GetIndex()];
        Resolver::shotsMissedSave[target->GetIndex()] = Resolver::shotsMissed[target->GetIndex()];
    }

    Resolver::lbyUpdated = isMoving || serverTime == lastLbyUpdate + 1.1f;

    if ( Resolver::lbyUpdated ) {
        lastLbyUpdate = serverTime;
    }

    if ( Resolver::lbyUpdated || Backtracking::backtrackingLby ) {
        angle.y = *target->GetLowerBodyYawTarget();
    } else {
        // Call the pCode here
        // Call HugBrute(), etc. so we have clean code and not messy like before
        // For example
        // angle.y = HugLby( target );
    }

    if ( didDmg ) {
        // You can save angles here, etc...

        didDmg = false;
    }

    if ( Settings::Resolver::resolvePitch ) {
        angle.x = HugPitch( target );
    }

    Math::NormalizePitch( angle.x );
    Math::NormalizeYaw( angle.y );

    target->GetEyeAngles()->x = angle.x;
    target->GetEyeAngles()->y = angle.y;
}

float Resolver::HugLby( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    studiohdr_t* hdr = modelInfo->GetStudioModel( target->GetModel() );

    if ( hdr && hdr->pSeqdesc( target->GetSequence() )->activity == 979 ) { // Let's use 979 instead of ACT_CSGO_IDLE_TURN_BALANCEADJUST, just to be sure
        // LBY broken and is between LBY+120 & LBY-120
        // Giving us range of 60 + 60 = 120
        // We should force 180 first, cuz its the most common one AFAIK
        angle.y = *target->GetLowerBodyYawTarget() + 180;
    } else {
        angle.y = *target->GetLowerBodyYawTarget();
    }

    return angle.y;
}

float Resolver::HugBrute( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();
    float lby = *target->GetLowerBodyYawTarget();

    switch ( Resolver::shotsMissedSave[target->GetIndex()] % 4 ) {
        case 0:
            angle.y = lby;
            break;
        case 1:
            angle.y = lby + 90.f;
            break;
        case 2:
            angle.y = lby - 90.f;
            break;
        case 3:
            angle.y = lby + 180.f;
            break;
    }

    return angle.y;
}

float Resolver::HugPitch( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    return angle.x;
}

////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\
///                   HOOKS                   \\\
////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\

void Resolver::FireGameEvent( IGameEvent* event ) {
    if ( !Settings::Resolver::enabled || !event ) {
        return;
    }

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() ) {
        return;
    }

    if ( strcmp( event->GetName(), XORSTR( "player_hurt" ) ) ) {
        C_BasePlayer* hurt = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );
        C_BasePlayer* attacker = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "attacker" ) ) )
        );

        if ( !hurt || !attacker ) {
            return;
        }

        if ( hurt == pLocal || attacker != pLocal ) {
            return;
        }

        didDmg = true;
        Resolver::shotsMissed[Resolver::resolvingId] = 0;
        lastAmmo = -1;
    }

    if ( strcmp( event->GetName(), XORSTR( "bullet_impact" ) ) ) {
        C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
                pLocal->GetActiveWeapon() );

        int ammo = activeWeapon->GetAmmo();
        if ( lastAmmo == -1 ) {
            lastAmmo = ammo;
        }

        if ( lastAmmo != ammo ) {
            Resolver::shotsMissed[Resolver::resolvingId]++;
            lastAmmo = ammo;
        }

        if ( strcmp( event->GetName(), XORSTR( "player_connect_full" ) ) ||
             strcmp( event->GetName(), XORSTR( "cs_game_disconnected" ) ) ) {

    }
}

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
            // If we should use Aimbot target
            if ( Aimbot::useAbTarget ) {
                C_BasePlayer* abTarget = ( C_BasePlayer* ) entityList->GetClientEntity( Aimbot::targetAimbot );
                Hug ( abTarget );
            } else {
                // If not, loop trough the entity list
                for ( int i = 1; i < engine->GetMaxClients(); i++ ) {
                    C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

                    if ( !target || target == pLocal || target->GetDormant() || !target->GetAlive() ||
                         target->GetImmune() || target->GetTeam() == pLocal->GetTeam() ) {
                        continue;
                    }
        
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
