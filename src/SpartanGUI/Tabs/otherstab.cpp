#include "otherstab.h"

void Others2::RenderTab() {

    // Aimbot::RenderTab();
    /* static char nickname[127] = "";
     static int page = 0;
             const char* tabs[] = {
                             ICON_SKINCHANGER,
                             ICON_PLAYERLIST,
             };
        
		
                     for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
         {
                     int distance = i == page ? 0 : i > page ? i - page : page - i;

                     ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
                                     Settings::UI::mainColor.Color().Value.x - (distance * 0.035f),
                                     Settings::UI::mainColor.Color().Value.y - (distance * 0.035f),
                                     Settings::UI::mainColor.Color().Value.z - (distance * 0.035f),
                                     Settings::UI::mainColor.Color().Value.w
                     );

         ImGui::GetStyle().Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();
            
                     if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 80)))
                             page = i;


                     if (i < IM_ARRAYSIZE(tabs) - 1)
             ImGui::SameLine();
				
         }
        
                     switch (page)
             {
                     case 0:
                             SkinModelChanger::RenderTab();
                             break;
                     case 1:
                             PlayerList::RenderTab();
                             break;
             }*/
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
    ImGui::Spacing();
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
