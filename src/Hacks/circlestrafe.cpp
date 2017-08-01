/*#include "circlestrafe.h"
bool Settings::CircleStrafe::enabled;
ButtonCode_t Settings::CircleStrafe::key = ButtonCode_t::KEY_C;

static void doCircleStrafe (CUserCmd* cmd, Vector& vecOriginalView) 
{ 
    
    	C_BasePlayer* pLocalEntity = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	

	static int iCircleFact = 0;
	iCircleFact++;
	if (iCircleFact > 361)
		iCircleFact = 0;

	float flRotation = 3.f * iCircleFact - globalVars->interval_per_tick;

        Vector StoredViewAngles = cmd->viewangles;

	if (inputSystem->IsButtonDown(Settings::CircleStrafe::key)) {
		cmd->forwardmove = 450.f;
		cmd->viewangles = vecOriginalView;
		flRotation = DEG2RAD(flRotation);

		float cos_rot = cos(flRotation);
		float sin_rot = sin(flRotation);

		float new_forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
		float new_sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

		cmd->forwardmove = new_forwardmove;
		cmd->sidemove = new_sidemove;
	}
    
}
void CircleStrafe::CreateMove(CUserCmd* cmd)
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	doCircleStrafe(cmd,???)
}
*/