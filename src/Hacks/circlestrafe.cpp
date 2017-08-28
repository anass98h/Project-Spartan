#include "circlestrafe.h"
bool Settings::CircleStrafe::enabled;
ButtonCode_t Settings::CircleStrafe::key = ButtonCode_t::KEY_C;

void CircleStrafe::CreateMove(CUserCmd* cmd) {
    C_BasePlayer* Spartan = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!Spartan || !Spartan->GetAlive())
        return;

    if (Spartan->GetMoveType() == MOVETYPE_LADDER || Spartan->GetMoveType() == MOVETYPE_NOCLIP)
        return;
    if(!Settings::CircleStrafe::enabled){return;}
    
    
    QAngle OView = *Spartan->GetVAngles();
    static int Circle = 0;
    Circle++;
    if (Circle > 361)
        Circle = 0;

    float SpinBoy = 3.f * Circle - globalVars->interval_per_tick;



    if (inputSystem->IsButtonDown(Settings::CircleStrafe::key)) {
        cmd->forwardmove = 450.f;
        cmd->viewangles = OView;
        SpinBoy = DEG2RAD(SpinBoy);

        float cos_rot = cos(SpinBoy);
        float sin_rot = sin(SpinBoy);

        float newforwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
        float newsidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

        cmd->forwardmove = newforwardmove;
        cmd->sidemove = newsidemove;
    }


}
