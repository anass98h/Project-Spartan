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

    float yawDiff = cmd->viewangles.y - *pLocal->GetLowerBodyYawTarget();
    Math::NormalizeYaw(yawDiff);
    normalYawDiff = yawDiff;
}

void lbyindicator::Paint()
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
    static Vertex_t indicatorVertexes[6];

    // Seemed a bit slow, so did some autistic tweaks
    if( width == 0 )
    {
        engine->GetScreenSize(width, height);
        radarCenterX = width * .23;
        radarCenterY = height* .42;
        radarNorthEndY = radarCenterY - 55;
        radar125EndX = 55 * cos(DEG2RAD(90+35)) + radarCenterX;
        radar125EndY = height*.42 - (55 * sin(DEG2RAD(90+35)));
        radar55EndX = 55 * cos(DEG2RAD(90-35)) + radarCenterX;
        radar55EndY = height*.42 - (55 * sin(DEG2RAD(90-35)));

        indicatorVertexes[0].Init(Vector2D(width * .259f, height * .470f));
        indicatorVertexes[1].Init(Vector2D(width * .257f, height * .475f));
        indicatorVertexes[2].Init(Vector2D(width * .253f, height * .475f));
        indicatorVertexes[3].Init(Vector2D(width * .251f, height * .470f));
        indicatorVertexes[4].Init(Vector2D(width * .253f, height * .463f));
        indicatorVertexes[5].Init(Vector2D(width * .257f, height * .463f));
    }

	float realYawDiff = normalYawDiff;

	if( AntiAim::isAntiAiming )
	{
		realYawDiff = *pLocal->GetLowerBodyYawTarget() - AntiAim::lastRealYaw;
		float fakeYawDiff = (*pLocal->GetLowerBodyYawTarget()) - AntiAim::lastFakeYaw;
		Math::NormalizeYaw(realYawDiff);
		Math::NormalizeYaw(fakeYawDiff);
		Draw::Line(radarCenterX, radarCenterY, 55 * cos(DEG2RAD(90-fakeYawDiff)) + radarCenterX, radarCenterY - (55 * sin(DEG2RAD(90-fakeYawDiff))), Color(10, 10, 200)); // Fake Yaw Line
	}
	if(std::abs(realYawDiff) >= 35.0f)
	{
		col = Color(0, 235, 0);
	}
	else
	{
		col = Color(196, 5, 5);
	}

	Draw::OutlinedCircle(radarCenterX, radarCenterY, 55, 16, Color(0,0,0)); // Radar Outline
	Draw::TexturedPolygon(6, indicatorVertexes, col); // Lby indicator light
	Draw::Line(radarCenterX, radarCenterY, radarCenterX, radarNorthEndY, Color(0,0,0)); // Const Line facing North
	Draw::Line(radarCenterX, radarCenterY, radar125EndX, radar125EndY, Color(0,0,0)); // Const Line Left-of N-Line
	Draw::Line(radarCenterX, radarCenterY, radar55EndX, radar55EndY, Color(0,0,0)); // Const Line Right-of
	Draw::Line(radarCenterX, radarCenterY, 55 * cos(DEG2RAD(90-realYawDiff)) + radarCenterX, radarCenterY - (55 * sin(DEG2RAD(90-realYawDiff))), col); // Real Yaw Line

	char lbyText[5]; // 3 digits, sign, and null terminator
	snprintf(lbyText, 5, "%.0f", (*pLocal->GetLowerBodyYawTarget()));
	Draw::Text(radarCenterX, radarNorthEndY - 20, lbyText, lby_font, Color(0, 0, 0)); // LBY On top
}




