#include "hvhtab.h"

int Settings::spinFactor::value = 1;
int Settings::customYaw::value = 1;
int Settings::customYaw2::value = 1;
bool Settings::customYaw::lby;
bool Settings::customYaw2::lby;

<<<<<<< HEAD
void HvH::RenderTab() {

    const char *yTypes[] = {
            "noaa", "Forwards", "Backwards", "Spin", "FakeSideWays", "Backjitter", "FJitter", "Rasp's EAP 1", "Rasp's EAP 2", "LBYJitter", "LBYSpin", "LBY",
            "LBY Breaker",
            "Fake LBY", "LBYonGROUND", "TANK1", "TANK2", "TANK3", "myrrib's eap", "richi's eap", "LegitTrolling 1", "LegitTrolling 2", "Custom 1", "Custom 2"  // safe
    // someone is retarded and pushes updates without double-checking them :dumb_some1:
    };

    const char *lbyTypes[] = {
            "Breaker 1", "Breaker 2", "Breaker 3", "Avoz", "Myrrib", "Myrrib 2", "NONE"
    };

    const char *zTypes[] = {
            "TEST", // not so untrusted
    };

    const char *xTypes[] = {
            "Up", "Flip", "Down", "Fake Zer0", "Fake Up",
    };

    const char *Hugtypes[] = {
            "OFF", "PasteHub", "Brute Log EAP", "Bruteforce", "TapTipTopTupTep",
            "Myrrib's DELTA", "Myrrib Old"
    };


    ImGui::Columns(2, NULL, true);
    {
        ImGui::BeginChild(XORSTR("HVH1"), ImVec2(0, 0), true);
        {
            ImGui::Text(XORSTR("AntiAim"));
            ImGui::BeginChild(XORSTR("##ANTIAIM"), ImVec2(0, 0), true);
            {
                ImGui::Checkbox(XORSTR("Yaw"), &Settings::AntiAim::Yaw::enabled);

                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Fake"));
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Fake Add"));
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Real"));
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Real Add"));
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Checkbox(XORSTR("Lby Breaker"), &Settings::AntiAim::Lby::enabled);
                    ImGui::Checkbox(XORSTR("Anti Resolver"), &Settings::AntiAim::Yaw::antiResolver);
                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth(-3);

                    ImGui::Combo(XORSTR("##YFAKETYPE"), (int *) &Settings::AntiAim::Yaw::typeFake, yTypes,
                                 IM_ARRAYSIZE(yTypes));
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::SliderFloat(XORSTR("##yawCustomFake"), &Settings::AntiAim::Yaw::customTypeFake,
                                           Settings::AntiAim::allowUntrustedAngles ? -1000 : -180,
                                           Settings::AntiAim::allowUntrustedAngles ? 1000 : 180,
                                           XORSTR(Settings::AntiAim::Yaw::customTypeFake > 0 ? "+%.1f" : "%.1f"));
                        ImGui::PopItemWidth();
                    }

                    ImGui::PushItemWidth(-3);

                    ImGui::Combo(XORSTR("##YACTUALTYPE"), (int *) &Settings::AntiAim::Yaw::type, yTypes,
                                 IM_ARRAYSIZE(yTypes));
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::SliderFloat(XORSTR("##yawCustom"), &Settings::AntiAim::Yaw::customType,
                                           Settings::AntiAim::allowUntrustedAngles ? -1000 : -180,
                                           Settings::AntiAim::allowUntrustedAngles ? 1000 : 180,
                                           XORSTR(Settings::AntiAim::Yaw::customType > 0 ? "+%.1f" : "%.1f"));
                        ImGui::PopItemWidth();
                    }
=======
static int page = 0;
>>>>>>> 4562470f9636873104ecd96d10fb36058d2d7196

const char* tabs[] = {
        "Standing", "Moving", "Airborne", "Miscellaneous"
};

void HvH::RenderTab() {
    for ( int i = 0; i < IM_ARRAYSIZE( tabs ); i++ ) {
        int distance = i == page ? 0 : i > page ? i - page : page - i;

        ImVec4 previousCol = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
                Settings::UI::mainColor.Color().Value.x - ( distance * 0.035f ),
                Settings::UI::mainColor.Color().Value.y - ( distance * 0.035f ),
                Settings::UI::mainColor.Color().Value.z - ( distance * 0.035f ),
                Settings::UI::mainColor.Color().Value.w
        );

        if ( ImGui::Button( tabs[i], ImVec2( ImGui::GetWindowSize().x / IM_ARRAYSIZE( tabs ) - 9, 0 ) ) ) {
            page = i;
        }

        ImGui::GetStyle().Colors[ImGuiCol_Button] = previousCol;

        if ( i < IM_ARRAYSIZE( tabs ) - 1 ) {
            ImGui::SameLine();
        }
    }

    ImGui::Separator();

    switch ( page ) {
        case 0:
            Standing::RenderTab();
            break;
        case 1:
            Moving::RenderTab();
            break;
        case 2:
            Airborne::RenderTab();
            break;
        case 3:
            MiscHvHTab::RenderTab();
            break;
    }
}
