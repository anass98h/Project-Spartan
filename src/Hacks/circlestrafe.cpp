#include "circlestrafe.h"

bool Settings::CircleStrafe::enabled;
ButtonCode_t Settings::CircleStrafe::key = ButtonCode_t::KEY_C;

// Maybe implement this as better version as rasp told me to implement a better cstrafer
// https://www.unknowncheats.me/forum/counterstrike-global-offensive/232136-adaptive-circlestrafer.html

void CircleStrafe::CreateMove( CUserCmd* cmd ) {
    C_BasePlayer* Spartan = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !Spartan || !Spartan->GetAlive() )
        return;

    if ( Spartan->GetMoveType() == MOVETYPE_LADDER || Spartan->GetMoveType() == MOVETYPE_NOCLIP )
        return;
    if ( !Settings::CircleStrafe::enabled ) { return; }


    QAngle OView = *Spartan->GetVAngles();
    static int Circle = 0;
    Circle++;
    if ( Circle > 361 )
        Circle = 0;

    float rotation = 3.f * Circle - globalVars->interval_per_tick;

    QAngle storedview = cmd->viewangles;

    if ( inputSystem->IsButtonDown( Settings::CircleStrafe::key ) ) {
        cmd->forwardmove = 450.f;
        cmd->viewangles = OView;


        rotation = DEG2RAD( rotation );

        float cos_rot = cos( rotation );
        float sin_rot = sin( rotation );

        float new_forwardmove = ( cos_rot * cmd->forwardmove ) - ( sin_rot * cmd->sidemove );
        float new_sidemove = ( sin_rot * cmd->forwardmove ) + ( cos_rot * cmd->sidemove );

        cmd->forwardmove = new_forwardmove;
        cmd->sidemove = -new_sidemove;

    }


}
