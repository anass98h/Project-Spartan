#include "lbyindicator.h"


bool Settings::lbyindicator::enabled = false;
static float normalYawDiff;
static Color col;

void lbyindicator::CreateMove(CUserCmd *cmd)
{
    if (!Settings::lbyindicator::enabled || !engine->IsInGame())
        return;

    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    if (!pLocal || !pLocal->GetAlive())
        return;

    float yawDiff = *pLocal->GetLowerBodyYawTarget() - cmd->viewangles.y;
    Math::NormalizeYaw(yawDiff);
    normalYawDiff = yawDiff;
}

void lbyindicator::PaintImGui()
{
	if (!Settings::lbyindicator::enabled || !engine->IsInGame())
		return;

	C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

	if (!pLocal || !pLocal->GetAlive())
		return;

	static int width = 0;
	static int height = 0;
	static int radarCenterX = 0;
	static int radarCenterY = 0;
	static int radarNorthEndY = 0;
	static int radar125EndX = 0; // 90 + 35
	static int radar125EndY = 0;
	static int radar55EndX = 0; // 90 - 35
	static int radar55EndY = 0;

	// Seemed a bit slow, so did some autistic tweaks
	if( width == 0 )
	{
		engine->GetScreenSize(width, height);
		radarCenterX = width * .3;
		radarCenterY = height* .5;
		radarNorthEndY = radarCenterY - 55;
		radar125EndX = 55 * cos(DEG2RAD(90+35)) + radarCenterX;
		radar125EndY = height*.5 - (55 * sin(DEG2RAD(90+35)));
		radar55EndX = 55 * cos(DEG2RAD(90-35)) + radarCenterX;
		radar55EndY = height*.5 - (55 * sin(DEG2RAD(90-35)));
	}

	float realYawDiff = normalYawDiff;

	if( AntiAim::isAntiAiming )
	{
		realYawDiff = *pLocal->GetLowerBodyYawTarget() - AntiAim::lastRealYaw;
		float fakeYawDiff = (*pLocal->GetLowerBodyYawTarget()) - AntiAim::lastFakeYaw;
		Math::NormalizeYaw(realYawDiff);
		Math::NormalizeYaw(fakeYawDiff);
		Draw::ImLine(ImVec2(radarCenterX, radarCenterY), ImVec2(55 * cos(DEG2RAD(90-fakeYawDiff)) + radarCenterX, radarCenterY - (55 * sin(DEG2RAD(90-fakeYawDiff)))), ImColor(10,10,200));
	}
	if(std::abs(realYawDiff) >= 35.0f)
	{
		col = Color(0, 235, 0);
	}
	else
	{
		col = Color(196, 5, 5);
	}

	Draw::ImCircle(ImVec2(radarCenterX, radarCenterY), ImColor(0,0,0), 55, 32); // Radar Outline
	Draw::ImCircleFilled(ImVec2(radarCenterX + 55, radarCenterY + 55), ImColor(col.r, col.g, col.b), 5, 16);  // LBY indicator Light
	Draw::ImLine(ImVec2(radarCenterX, radarCenterY), ImVec2(radarCenterX, radarNorthEndY), ImColor(0,0,0)); // Const Line facing North
	Draw::ImLine(ImVec2(radarCenterX, radarCenterY), ImVec2(radar125EndX, radar125EndY), ImColor(0,0,0)); // Const Line Left-of N-Line
	Draw::ImLine(ImVec2(radarCenterX, radarCenterY), ImVec2(radar55EndX, radar55EndY), ImColor(0,0,0)); // Const Line Right-of N-Line
	Draw::ImLine(ImVec2(radarCenterX, radarCenterY), ImVec2(55 * cos(DEG2RAD(90-realYawDiff)) + radarCenterX,radarCenterY - (55 * sin(DEG2RAD(90-realYawDiff)))), ImColor(col.r, col.g, col.b)); // Real Yaw Line

	char lbyText[5]; // 3 digits, sign, and null terminator
	snprintf(lbyText, 5, "%.0f", (*pLocal->GetLowerBodyYawTarget()));
	Draw::ImText(ImVec2(radarCenterX, radarNorthEndY - 20), ImColor(0,0,0), lbyText); // LBY On top
}



