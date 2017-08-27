#include "angleindicator.h"

bool Settings::AngleIndicator::enabled = false;
static float fakeAngle;
static float realAngle;

void AngleIndicator::PostPredictionCreateMove(CUserCmd* cmd) {
    if (!Settings::AngleIndicator::enabled || !engine->IsInGame())
        return;

    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!pLocal || !pLocal->GetAlive())
        return;

    if(Settings::AntiAim::Yaw::enabled) {
        fakeAngle = AntiAim::lastFakeYaw;
        realAngle = AntiAim::lastRealYaw;
    } else {
        fakeAngle = 0;
        realAngle = cmd->viewangles.y;
    }
}

void AngleIndicator::PaintImGui() {
    if (!Settings::AngleIndicator::enabled || !engine->IsInGame())
        return;

    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!pLocal || !pLocal->GetAlive())
        return;

    // Creation of the text

    std::ostringstream stream;
    stream << Math::RoundFloat(realAngle);
    std::string angleText(stream.str());

    std::string text(XORSTR("Real: "));
    text.append(angleText);

    std::ostringstream stream2;
    stream2 << Math::RoundFloat(fakeAngle);
    std::string fakeText(stream2.str());
    std::string text2(XORSTR("Fake: "));
    text2.append(fakeText);

    // Calculation of text position on screen

    int width;
    int height;
    engine->GetScreenSize(width, height);

    int textX = (width - (width - (width * 85 / 100)));
    int textY = height - (height - (height * 53 / 100));
    int textX2 = (width - (width - (width * 85 / 100)));
    int textY2 = height - (height - (height * 51 / 100));

    Draw::ImText(ImVec2(textX, textY), ImColor(255, 255, 255), text.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
    Draw::ImText(ImVec2(textX2, textY2), ImColor(255, 255, 255), text2.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
}
