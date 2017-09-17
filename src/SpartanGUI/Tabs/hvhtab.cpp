#include "hvhtab.h"

int Settings::spinFactor::value = 1;
int Settings::customYaw::value = 1;
int Settings::customYaw2::value = 1;
bool Settings::customYaw::lby;
bool Settings::customYaw2::lby;

static int page = 0;

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

        if ( ImGui::Button( tabs[i], ImVec2( ImGui::GetWindowSize().x / IM_ARRAYSIZE( tabs ) - 9, 0 ))) {
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
