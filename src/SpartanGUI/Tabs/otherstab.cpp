#include "otherstab.h"

void Others2::RenderTab() {

    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing(); // p
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    //Align the labels centered to the checkboxes
    //Set Column flags to not dragable
    ImGui::Columns(6, "Playerlist?", false);
    {
        ImGui::Text(ICON_PLAYERLIST);
        ImGui::Checkbox("PlayerList", &PlayerList::showWindow);
    }

    ImGui::SameLine();

    ImGui::NextColumn();
    {
        ImGui::Text(ICON_SKINCHANGER);
        ImGui::Checkbox("Skinchanger", &SkinModelChanger::showWindow);
    }

    ImGui::SameLine();

    ImGui::NextColumn();
    {
        ImGui::Text(ICON_SPECTATORS);
        ImGui::Checkbox("Spectator", &Settings::ShowSpectators::enabled);
    }

    ImGui::SameLine();

    ImGui::NextColumn();
    {
        ImGui::Text(ICON_COLOURS, ImVec2(100, 0));
        ImGui::Checkbox("Colors", &Colors::showWindow);
    }

    ImGui::SameLine();

    ImGui::NextColumn();
    {
        ImGui::Text(ICON_CONFIGS, ImVec2(ImGui::CalcTextSize(ICON_CONFIGS).x, 0));
        ImGui::Checkbox("Configs", &Configs::showWindow);
    }



}


/*
 
#define ICON_SPECTATORS u8"\ue04B"
#define ICON_WALKBOT u8"\ue04C"
#define ICON_SKINCHANGER u8"\ue04D"
#define ICON_PLAYERLIST u8"\ue04E"
#define ICON_RAGE u8"\ue04F"
#define ICON_CONFIGS u8"\ue050"
#define ICON_COLORS u8"\ue051"

 */
