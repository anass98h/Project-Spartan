#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::resolvePitch = false;
bool Settings::Resolver::lagCompensation = false;
bool Settings::Resolver::headshotLbyUpdateOnly = false;
bool Settings::Resolver::baimNextShot = false;
bool Settings::AngleFlip::enabled = false;
ButtonCode_t Settings::AngleFlip::key = ButtonCode_t::KEY_F;

// Local variables
bool didDmg = false;
std::map<int, int> shotsMissed = { { -1, 0 } };
std::map<int, int> shotsMissedSave = { { -1, 0 } };
static int lastAmmo = -1;

// Global variables
bool Settings::Resolver::lbyUpdated = false;
int Settings::Resolver::resolvingId = -1;
std::map<int, float> Settings::Resolver::lby = {
    { -1, 0 }
};
std::map<int, float> Settings::Resolver::lastHitAng = {
    { -1, 0 }
};
std::map<int, float> Settings::Resolver::angForce = {
    { -1, 0 }
};
std::map<int, const char*> Settings::Resolver::angForceTxt = {
    { -1, "None" }
};

void Resolver::Hug( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    Settings::Resolver::resolvingId = target->GetIndex();

    float velocity = target->GetVelocity().Length2D();
    bool onGround = target->GetFlags() & FL_ONGROUND;
    bool isMoving = onGround && velocity > 35.f;

    float serverTime = target->GetTickBase() * globalVars->interval_per_tick;
    float curTime = globalVars->curtime;
    float lastLbyUpdate = 0.0f;

    float lby = *target->GetLowerBodyYawTarget();
    static float lbySave = lby;

    float shotsMissedTime = 2.0f;
    float lastShotsMissed = 0.0f;

    // Set maps in here
    Settings::Resolver::lby[target->GetIndex()] = lby;

    static int shotsMissedS = shotsMissed[target->GetIndex()];

    if ( shotsMissed[target->GetIndex()] != shotsMissedS && shotsMissed[target->GetIndex()] != 0 ) {
        shotsMissedS++;
        shotsMissedSave[target->GetIndex()]++;
        lastShotsMissed = curTime;
    }  if ( curTime > lastShotsMissed + shotsMissedTime ) {
        shotsMissedS = shotsMissed[target->GetIndex()];
        shotsMissed[target->GetIndex()] = shotsMissed[target->GetIndex()];
    }

    Settings::Resolver::lbyUpdated = ( isMoving || serverTime == lastLbyUpdate + 1.1f || lby != lbySave );

    if ( lby != lbySave )
        lbySave = lby;

    if ( Settings::Resolver::lbyUpdated )
        lastLbyUpdate = serverTime;

    if ( Settings::Resolver::lbyUpdated || Backtracking::backtrackingLby ) {
        angle.y = lby;
        Settings::Resolver::angForce[target->GetIndex()] = lby;
        Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY";
    } else {
        // Call the pCode here
        // Call HugBrute(), etc. so we have clean code and not messy like before
        // For example
        //angle.y = HugLby(target);
        angle.y = HugBrute( target );
    }

    // THIS MUST BE AFTER WE SET ALL ANGLES!!!
    if ( didDmg ) {
        // You can save angles here, etc...
        Settings::Resolver::lastHitAng[target->GetIndex()] = angle.y;

        didDmg = false;
    }

    if ( Settings::Resolver::resolvePitch )
        angle.x = HugPitch( target );
    
    if(!onGround)
        Settings::Resolver::baimNextShot = true;
    
    Math::NormalizePitch( angle.x );
    Math::NormalizeYaw( angle.y );

    target->GetEyeAngles()->x = angle.x;
    target->GetEyeAngles()->y = angle.y;
}

float Resolver::HugLby( C_BasePlayer* target ) { //TODO: Fix crashing of this
    QAngle angle = *target->GetEyeAngles();
    float lby = *target->GetLowerBodyYawTarget();

    studiohdr_t* hdr = modelInfo->GetStudioModel( target->GetModel() );

    if ( hdr && hdr->pSeqdesc( target->GetSequence() )->activity == 979 ) { // Let's use 979 instead of ACT_CSGO_IDLE_TURN_BALANCEADJUST, just to be sure
        // LBY broken and is between LBY+120 & LBY-120
        // Giving us range of 60 + 60 = 120
        // We should force 180 first, cuz its the most common one AFAIK
        angle.y = lby + 180;
        Settings::Resolver::angForce[target->GetIndex()] = lby + 180;
        Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY + 180";
    } else {
        angle.y = lby;
        Settings::Resolver::angForce[target->GetIndex()] = lby;
        Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY";
    }

    return angle.y;
}

float Resolver::HugBrute( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();
    float lby = *target->GetLowerBodyYawTarget();

    switch ( shotsMissed[target->GetIndex()] % 4 ) {
        case 0:
            angle.y = lby;
            Settings::Resolver::angForce[target->GetIndex()] = lby;
            Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY";
            break;
        case 1:
            angle.y = lby + 90.f;
            Settings::Resolver::angForce[target->GetIndex()] = lby + 90.f;
            Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY + 90";
            break;
        case 2:
            angle.y = lby - 90.f;
            Settings::Resolver::angForce[target->GetIndex()] = lby - 90.f;
            Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY - 90";
            break;
        case 3:
            angle.y = lby + 180.f;
            Settings::Resolver::angForce[target->GetIndex()] = lby + 180.f;
            Settings::Resolver::angForceTxt[target->GetIndex()] = "LBY + 180";
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
        shotsMissed[Settings::Resolver::resolvingId] = 0;
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
            shotsMissed[Settings::Resolver::resolvingId]++;
            lastAmmo = ammo;
        }

        if ( strcmp( event->GetName(), XORSTR( "player_connect_full" ) ) ||
             strcmp( event->GetName(), XORSTR( "cs_game_disconnected" ) ) ) {

        }
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
