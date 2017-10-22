#include "walkbot.h"

std::map<std::string, WalkBot::WalkBotPointInfo> WalkBot::map = std::map<std::string, WalkBot::WalkBotPointInfo>();
bool WalkBot::shouldForceReset = false;

unsigned int rally = 0;
static bool reachedEnd = false;
static bool hasAutoBought = false;
static bool hasDied = false;
static bool slowLastTick = false;

void WalkBot::CreateMove( CUserCmd* cmd ) {


}

bool Rally( std::vector<Vector> points, C_BasePlayer* pLocal, CUserCmd* cmd ) {
    if ( reachedEnd ) {
        return true;
    }

    if ( ( std::abs( pLocal->GetEyePosition().x - points[rally].x ) < 0.6f ) &&
         ( std::abs( pLocal->GetEyePosition().y - points[rally].y ) < 0.6f ) ) {
        if ( rally == ( points.size() - 1 ) ) {
            if ( !reachedEnd ) {
                cvar->ConsoleColorPrintf( ColorRGBA( 255, 255, 255 ), XORSTR( "Reached Rally #%d \n" ), rally );
                cvar->ConsoleColorPrintf( ColorRGBA( 0, 255, 0 ), XORSTR( "All rally points have been reached." ) );
                reachedEnd = true;
            }

            return reachedEnd;
        }

        cvar->ConsoleColorPrintf( ColorRGBA( 255, 255, 255 ), XORSTR( "Reached Rally #%d \n" ), rally );
        rally++;
        cvar->ConsoleColorPrintf( ColorRGBA( 255, 255, 255 ), XORSTR( "Continuing to Rally #%d \n" ), rally );
    }

    QAngle angle = Math::CalcAngle( pLocal->GetEyePosition(), points[rally] );
    cmd->forwardmove = 255.0f;
    cmd->sidemove = 0.0f;
    cmd->buttons |= IN_WALK;
    Math::CorrectMovement( angle, cmd, cmd->forwardmove, cmd->sidemove );

    return false;
}

void WalkBot::ForceReset() {
    if ( reachedEnd || WalkBot::shouldForceReset || hasDied ) {
        rally = 0;
        reachedEnd = false;
        WalkBot::shouldForceReset = false;
        hasDied = false;
    }
}

