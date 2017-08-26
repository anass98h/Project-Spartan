#include "angleindicator.h"
#include "antiaim.h"

bool Settings::AngleIndicator::enabled = false;
static float fakeAngle;
static float realAngle;

void AngleIndicator::PostPredictionCreateMove(CUserCmd* cmd) {
    if(!Settings::AngleIndicator::enabled || !engine->IsInGame())
        return;

    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    if(!pLocal || !pLocal->GetAlive())
        return;
        fakeAngle = AntiAim::lastFakeYaw;
        realAngle = AntiAim::lastRealYaw;
}

void AngleIndicator::Paint() {
    if(!Settings::AngleIndicator::enabled || !engine->IsInGame())
        return;
    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!pLocal || !pLocal->GetAlive())
         return;
    
    if(realAngle == -1337) // what even is this shit ?
        bool bullshit = true;

    // Creation of the text

    std::ostringstream stream;
    stream << realAngle;
    std::string angleText(stream.str());

    std::string text(XORSTR("Real: "));
    text.append(angleText);
    
    std::ostringstream stream2; 
    stream2 << fakeAngle;
    std::string fakeText(stream2.str());
    std::string text2(XORSTR("Fake: "));
    text2.append(fakeText);
    // Calculation of text position on screen

    int width;
    int height;
    engine->GetScreenSize(width, height);
    
    int textX = (width- (width - (width * 85 / 100)));
    int textY = height - (height - (height * 53 / 100));
    int textX2 = (width - (width - (width * 85 / 100)));
    int textY2 = height - (height - (height * 51 / 100));
    

    // TODO: Get the current fake angle and draw that above the current real angle
    
   // Draw::ImText(ImVec2(textX, textY), ImColor(244, 66, 83), text.c_str());
   // Draw::IMText(ImVec2(textX2, textY2), ImColor(244, 66, 83), text2.c_str());
    
     Draw::Text(textX, textY, text.c_str(), lby_font, Color(244, 66, 83));
     Draw::Text(textX2, textY2, text2.c_str(), lby_font, Color(244, 66, 83));
}

