#include "main.h"


bool Main::showWindow = false;

void Main::RenderWindow() {
    if ( !Main::showWindow )
        return;


    static int page = 0;
    ImGui::SetNextWindowSize( ImVec2( 1080, 700 ), ImGuiSetCond_FirstUseEver );
    if ( ImGui::Begin( ( "Project Spartan" ), &Main::showWindow,
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders ) ) {

        const char* tabs[] = {
                "  " ICON_7 " \n  Aim",

                "  " ICON_9 " \nVisuals",
                ICON_8 "\nHvH",
                ICON_12 "\nMisc"
        };


        ImGui::BeginChild( "Main1", ImVec2( 150, 0 ), true, ImGuiWindowFlags_NoResize );
        {
            for ( int i = 0; i < IM_ARRAYSIZE( tabs ); i++ ) {
                int distance = i == page ? 0 : i > page ? i - page : page - i;

                ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
                        Settings::UI::mainColor.Color().Value.x - ( distance * 0.035f ),
                        Settings::UI::mainColor.Color().Value.y - ( distance * 0.035f ),
                        Settings::UI::mainColor.Color().Value.z - ( distance * 0.035f ),
                        Settings::UI::mainColor.Color().Value.w
                );

                ImGui::GetStyle().Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();

                if ( ImGui::Button( tabs[i], ImVec2( ImGui::GetWindowSize().x - 9,
                                                     ImGui::GetWindowSize().y / IM_ARRAYSIZE( tabs ) - 5 ) ) )
                    page = i;


                i < IM_ARRAYSIZE( tabs ) - 1;

            }
            ImGui::EndChild();
        }

        ImGui::SameLine();
        ImGui::BeginChild( "Main2", ImVec2( 0, 0 ), true, ImGuiWindowFlags_NoResize );
        {


            switch ( page ) {
                case 0:
                    AimTab::RenderTab();
                    break;
                case 1:
                    Visuals::RenderTab();
                    break;
                case 2:
                    HvH::RenderTab();
                    break;
                case 3:
                    Misc::RenderTab();
                    break;
            }


            ImGui::EndChild();
        }

        ImGui::End();
    }

}


