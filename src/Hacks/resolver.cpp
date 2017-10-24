#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::resolvePitch = false;
bool Settings::Resolver::lagCompensation = false;
bool Settings::Resolver::headshotLbyUpdateOnly = false;
bool Settings::AngleFlip::enabled = false;
ButtonCode_t Settings::AngleFlip::key = ButtonCode_t::KEY_F;

// Local variables
bool didDmg = false;
std::map<int, int> shotsMissed = {
        { -1, 0 }
};
std::map<int, int> shotsMissedSave = {
        { -1, 0 }
};
static int lastAmmo = -1;

// Global variables
bool Resolver::lbyUpdated = false;
bool Resolver::baimNextShot = false;
int Resolver::resolvingId = -1;
std::map<int, float> Resolver::lby = {
        { -1, 0.0f }
};
std::map<int, float> Resolver::lastHitAng = {
        { -1, 0.0f }
};
std::map<int, float> Resolver::angForce = {
        { -1, 0.0f }
};
std::map<int, const char*> Resolver::angForceTxt = {
        { -1, "None" }
};
std::map<int, int> Resolver::shotsMissedSave = {
        { -1, 0 }
};

void Resolver::Hug( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    Resolver::resolvingId = target->GetIndex();

    float velocity = target->GetVelocity().Length2D();
    bool onGround = target->GetFlags() & FL_ONGROUND;
    bool isMoving = onGround && velocity > 35.0f;

    float serverTime = target->GetTickBase() * globalVars->interval_per_tick;
    float curTime = globalVars->curtime;
    float lastLbyUpdate = 0.0f;

    float lby = *target->GetLowerBodyYawTarget();
    static float lbySave = lby;

    float shotsMissedTime = 2.0f;
    float lastShotsMissed = 0.0f;

    // Set maps in here
    Resolver::lby[target->GetIndex()] = lby;

    static int shotsMissedS = shotsMissed[target->GetIndex()];

    if ( shotsMissed[target->GetIndex()] != shotsMissedS && shotsMissed[target->GetIndex()] != 0 ) {
        shotsMissedS = shotsMissed[target->GetIndex()];
        Resolver::shotsMissedSave[target->GetIndex()]++;
        lastShotsMissed = curTime;
    }
    
    if ( curTime > lastShotsMissed + shotsMissedTime ) {
        shotsMissedS = shotsMissed[target->GetIndex()];
        Resolver::shotsMissedSave[target->GetIndex()] = shotsMissed[target->GetIndex()];
    }

    Resolver::lbyUpdated = ( isMoving || serverTime == lastLbyUpdate + 1.1f || lby != lbySave );

    // If it is already the same value, we just set it again to the same. No need for an if-clause.
    lbySave = lby;

    if ( Resolver::lbyUpdated ) {
        lastLbyUpdate = serverTime;
    }

    if ( Resolver::lbyUpdated || Backtracking::backtrackingLby ) {
        angle.y = lby;
        Resolver::angForce[target->GetIndex()] = lby;
        Resolver::angForceTxt[target->GetIndex()] = "LBY";
    } else {
        // Call the pCode here
        // Call HugBrute(), etc. so we have clean code and not messy like before
        // For example
        angle.y = HugBrute( target );
    }

    if ( Settings::Resolver::resolvePitch ) {
        angle.x = HugPitch( target );
    }

    // THIS MUST BE AFTER WE SET ALL ANGLES!!!
    if ( didDmg ) {
        // You can save angles here, etc...
        Resolver::lastHitAng[target->GetIndex()] = angle.y;

        didDmg = false;
    }

    if ( !onGround ) {
        Resolver::baimNextShot = true;
    }
    
    Math::NormalizePitch( angle.x );
    Math::NormalizeYaw( angle.y );

    target->GetEyeAngles()->x = angle.x;
    target->GetEyeAngles()->y = angle.y;
}

float Resolver::HugLby( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();
    float lby = *target->GetLowerBodyYawTarget();

    // ldd -r Spartan.so complains that GetNumSeq() is not defined (used in pSeqdesc) - requires changing
    // Still injects tho!

    studiohdr_t* hdr = modelInfo->GetStudioModel( target->GetModel() );

    if ( hdr && hdr->pSeqdesc( target->GetSequence() )->activity == ACT_CSGO_IDLE_TURN_BALANCEADJUST ) {
        // LBY broken and is between LBY+120 & LBY-120
        // Giving us range of 60 + 60 = 120
        // We should force 180 first, cuz its the most common one AFAIK
        angle.y = lby + 180;
        Resolver::angForce[target->GetIndex()] = lby + 180;
        Resolver::angForceTxt[target->GetIndex()] = "LBY + 180";
    } else {
        angle.y = lby;
        Resolver::angForce[target->GetIndex()] = lby;
        Resolver::angForceTxt[target->GetIndex()] = "LBY";
    }

    return angle.y;
}

float Resolver::HugBrute( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();
    float lby = *target->GetLowerBodyYawTarget();

    switch ( Resolver::shotsMissedSave[target->GetIndex()] % 4 ) {
        case 0:
            angle.y = lby; // Nice Fuzion resolver you got there
            Resolver::angForce[target->GetIndex()] = lby;
            Resolver::angForceTxt[target->GetIndex()] = "LBY";
            break;
        case 1:
            angle.y = lby + 90.0f;
            Resolver::angForce[target->GetIndex()] = lby + 90.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY + 90";
            break;
        case 2:
            angle.y = lby - 90.0f;
            Resolver::angForce[target->GetIndex()] = lby - 90.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY - 90";
            break;
        case 3:
            // Didn't we brute this as first
            angle.y = lby + 180.0f;
            Resolver::angForce[target->GetIndex()] = lby + 180.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY + 180";
            break;
        case 4:
            angle.y = lby - 180.0f;
            Resolver::angForce[target->GetIndex()] = lby - 180.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY - 180";
            break;
    }

    return angle.y;
}

float Resolver::HugPitch( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    // TODO: NoSpread Pitch resolver

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
        shotsMissed[Resolver::resolvingId] = 0;
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
            shotsMissed[Resolver::resolvingId]++;
            lastAmmo = ammo;
        }
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
        case ClientFrameStage_t::FRAME_UNDEFINED:
        case ClientFrameStage_t::FRAME_START:
        case ClientFrameStage_t::FRAME_NET_UPDATE_START:
        case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END:
        case ClientFrameStage_t::FRAME_NET_UPDATE_END:
        case ClientFrameStage_t::FRAME_RENDER_START:
            // Not needed.
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
