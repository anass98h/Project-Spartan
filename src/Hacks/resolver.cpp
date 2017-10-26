#include "resolver.h"

bool Settings::Resolver::enabled = false;
bool Settings::Resolver::resolvePitch = false;
bool Settings::Resolver::lagCompensation = false;
bool Settings::Resolver::headshotLbyUpdateOnly = false;
bool Settings::AngleFlip::enabled = false;
ButtonCode_t Settings::AngleFlip::key = ButtonCode_t::KEY_F;

// Local variables
bool didDmg = false;
std::map<int, int> missedShotsMap = {};
static int lastAmmo = -1;

// Global variables
bool Resolver::lbyUpdated = false;
bool Resolver::baimNextShot = false;
int Resolver::resolvingId = -1;
std::map<int, float> Resolver::lby = {
    { -1, 0.0f }
};
std::map<int, bool> Resolver::hasFakeWalk = {
    { -1, false }
};
std::map<int, float> Resolver::lastHitAng = {
    { -1, 0.0f }
};
std::map<int, const char*> Resolver::lastHitAngTxt = {
    { -1, "None" }
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
std::map<int, Angle> Resolver::lastForceAng = {
    { -1, LBY }
};

void Resolver::Hug( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    Resolver::resolvingId = target->GetIndex();

    float velocity = target->GetVelocity().Length2D();
    bool onGround = target->GetFlags() & FL_ONGROUND;
    bool isMoving = onGround && velocity > 1.0f;

    float serverTime = target->GetTickBase() * globalVars->interval_per_tick;
    float curTime = globalVars->curtime;
    float lastLbyUpdate = 0.0f;

    float lby = *target->GetLowerBodyYawTarget();
    static float lbySave = lby;

    float shotsMissedTime = 2.0f;
    float lastShotsMissed = 0.0f;

    // Set maps in here
    Resolver::lby[target->GetIndex()] = lby;

    Resolver::lbyUpdated = ( isMoving || serverTime == lastLbyUpdate + 1.1f || lby != lbySave );

    // If it is already the same value, we just set it again to the same. No need for an if-clause.
    lbySave = lby;

    if ( Resolver::lbyUpdated )
        lastLbyUpdate = serverTime;

    if ( Settings::AngleFlip::enabled && inputSystem->IsButtonDown( Settings::AngleFlip::key ) ) {
        switch ( Resolver::lastForceAng[target->GetIndex()] ) {
            case Angle::LBY:
                angle.y = lby + 180;
                Resolver::angForce[target->GetIndex()] = lby + 180;
                Resolver::angForceTxt[target->GetIndex()] = "LBY + 180";
                break;
            case Angle::LBY180:
                angle.y = lby;
                Resolver::angForce[target->GetIndex()] = lby;
                Resolver::angForceTxt[target->GetIndex()] = "LBY";
                break;
            case Angle::LBYP90:
                angle.y = lby - 90;
                Resolver::angForce[target->GetIndex()] = lby - 90;
                Resolver::angForceTxt[target->GetIndex()] = "LBY - 90";
                break;
            case Angle::LBYM90:
                angle.y = lby + 90;
                Resolver::angForce[target->GetIndex()] = lby + 90;
                Resolver::angForceTxt[target->GetIndex()] = "LBY + 90";
                break;
        }
    } else {
        if ( Resolver::lbyUpdated || Backtracking::backtrackingLby ) {
            angle.y = lby;
            Resolver::angForce[target->GetIndex()] = lby;
            Resolver::angForceTxt[target->GetIndex()] = "LBY";
            Resolver::lastForceAng[target->GetIndex()] = Angle::LBY;
            if ( velocity < 100.0f && onGround ) {
                if ( missedShotsMap[target->GetIndex()] > 1 && Resolver::hasFakeWalk[target->GetIndex()] ) {
                    angle.y = HugBrute(target);
                } else if ( missedShotsMap[target->GetIndex()] > 2 && !Resolver::hasFakeWalk[target->GetIndex()] ) {
                    Resolver::hasFakeWalk[target->GetIndex()] = true;
                }
            }
        } else {
            // Call the pCode here
            // Call HugBrute(), etc. so we have clean code and not messy like before
            // For example
            //angle.y = HugLby(target);
            angle.y = HugBrute( target );
        }
    }

    if ( Settings::Resolver::resolvePitch ) {
        angle.x = HugPitch( target );
    }

    Resolver::baimNextShot = Resolver::shotsMissedSave[target->GetIndex()] > 4;

    if ( !onGround ) {
        Resolver::baimNextShot = true;
    }
    
    Math::NormalizePitch( angle.x );
    Math::NormalizeYaw( angle.y );

    // THIS MUST BE AFTER WE SET ALL ANGLES!!!
    if ( didDmg ) {
        // You can save angles here, etc...
        Resolver::lastHitAng[target->GetIndex()] = angle.y;
        Resolver::lastHitAngTxt[target->GetIndex()] = Resolver::angForceTxt[target->GetIndex()];

        didDmg = false;
    }

    target->GetEyeAngles()->x = angle.x;
    target->GetEyeAngles()->y = angle.y;
}

float Resolver::HugLby( C_BasePlayer* target ) { //TODO: Fix crashing of this
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
        Resolver::lastForceAng[target->GetIndex()] = Angle::LBY180;
    } else {
        angle.y = lby;
        Resolver::angForce[target->GetIndex()] = lby;
        Resolver::angForceTxt[target->GetIndex()] = "LBY";
        Resolver::lastForceAng[target->GetIndex()] = Angle::LBY;
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
            Resolver::lastForceAng[target->GetIndex()] = Angle::LBY;
            break;
        case 1:
            angle.y = lby + 180.0f;
            Resolver::angForce[target->GetIndex()] = lby + 180.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY + 180";
            Resolver::lastForceAng[target->GetIndex()] = Angle::LBY180;
            break;
        case 2:
            angle.y = lby - 90.0f;
            Resolver::angForce[target->GetIndex()] = lby - 90.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY - 90";
            Resolver::lastForceAng[target->GetIndex()] = Angle::LBYM90;
            break;
        case 3:
            angle.y = lby + 90.0f;
            Resolver::angForce[target->GetIndex()] = lby + 90.0f;
            Resolver::angForceTxt[target->GetIndex()] = "LBY + 90";
            Resolver::lastForceAng[target->GetIndex()] = Angle::LBYP90;
            break;
    }

    return angle.y;
}

float Resolver::HugPitch( C_BasePlayer* target ) {
    QAngle angle = *target->GetEyeAngles();

    // TODO: NoSpread Pitch resolver

    return angle.x;
}

/*
    hooks
*/

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
        missedShotsMap[hurt->GetIndex()] = 0;
        lastAmmo = -1;
    }

    if ( strcmp( event->GetName(), XORSTR( "bullet_impact" ) ) ) {
        C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
                pLocal->GetActiveWeapon() );

        C_BasePlayer* hurt = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );

        if(!hurt || !activeWeapon)
            return;

        float x = event->GetFloat(XORSTR("x"));

        bool didHit = false;

        for ( int i = 1; i < engine->GetMaxClients(); i++ ) {
            C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

            Vector vector = target->GetBonePosition((int)Bone::BONE_PELVIS);

            if(((vector.x - x) <= 10) || ((vector.x + x) <= 10)) {
                didHit = true;
            }
        }

        if(!didHit) {
            missedShotsMap[Aimbot::targetAimbot]++;
        }
    }

    if(strcmp(event->GetName(), XORSTR( "player_connect_full" ) )) {
        C_BasePlayer* player = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );

        missedShotsMap[player->GetIndex()] = 0;
    }

    if ( strcmp( event->GetName(), XORSTR( "cs_game_disconnected" ) ) ) {
        C_BasePlayer* player = ( C_BasePlayer* ) entityList->GetClientEntity(
                engine->GetPlayerForUserID( event->GetInt( XORSTR( "userid" ) ) )
        );

        if(missedShotsMap.find(player->GetIndex()) != missedShotsMap.end()) {
            missedShotsMap.erase(player->GetIndex());
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
                Hug( abTarget );
            } else {
                // If not, loop trough the entity list
                for ( int i = 1; i < engine->GetMaxClients(); i++ ) {
                    C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

                    if ( !target || target == pLocal || target->GetDormant() || !target->GetAlive() ||
                         target->GetImmune() || target->GetTeam() == pLocal->GetTeam() ) {
                        continue;
                    }
        
                    Hug( target );
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
