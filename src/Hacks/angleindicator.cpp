#include "angleindicator.h"

bool Settings::AngleIndicator::enabled = false;

float realAngle = -1337;

void AngleIndicator::PaintImGui() {
    if(!Settings::AngleIndicator::enabled || !engine->IsInGame())
        return;

    if(realAngle == -1337)
        return;

    // Creation of the text

    std::ostringstream stream;
    stream << Math::RoundFloat(realAngle);
    std::string angleText(stream.str());

    std::string text(XORSTR("Real: "));
    text.append(angleText);

    // Calculation of text position on screen

    int width;
    int height;
    engine->GetScreenSize(width, height);

    float textX = width - ( strlen(text.c_str()) * 7.f );
    float textY = height - ImGui::GetWindowFontSize() - 5.f;

    // TODO: Get the current fake angle and draw that above the current real angle

    Draw::ImText(ImVec2(textX, textY), ImColor(255, 255, 255), text.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
}

void AngleIndicator::PostPredictionCreateMove(CUserCmd* cmd) {
    if(!Settings::AngleIndicator::enabled || !engine->IsInGame())
        return;

    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    if(!pLocal || !pLocal->GetAlive())
        return;

    realAngle = cmd->viewangles.y;
}
