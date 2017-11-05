#include "main.h"


bool Main::showWindow = false;

void Main::RenderWindow() {
    if ( Settings::UI::Windows::Main::reload ) {
        ImGui::SetNextWindowPos( ImVec2( Settings::UI::Windows::Main::posX, Settings::UI::Windows::Main::posY ),
                                 ImGuiSetCond_Always );
        ImGui::SetNextWindowSize( ImVec2( Settings::UI::Windows::Main::sizeX, Settings::UI::Windows::Main::sizeY ),
                                  ImGuiSetCond_Always );
        Main::showWindow = Settings::UI::Windows::Main::open;
        Settings::UI::Windows::Main::reload = false;
    } else {
        ImGui::SetNextWindowPos( ImVec2( Settings::UI::Windows::Main::posX, Settings::UI::Windows::Main::posY ),
                                 ImGuiSetCond_FirstUseEver );
        ImGui::SetNextWindowSize( ImVec2( Settings::UI::Windows::Main::sizeX, Settings::UI::Windows::Main::sizeY ),
                                  ImGuiSetCond_FirstUseEver );
    }
    if ( !Main::showWindow ) {
        Settings::UI::Windows::Main::open = false;
        return;
    }


    static int page = 0;

    if ( ImGui::Begin( ( "Project Spartan" ), &Main::showWindow,
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders ) ) {
        Settings::UI::Windows::Main::open = true;
        ImVec2 temp = ImGui::GetWindowSize();
        Settings::UI::Windows::Main::sizeX = ( int ) temp.x;
        Settings::UI::Windows::Main::sizeY = ( int ) temp.y;
        temp = ImGui::GetWindowPos();
        Settings::UI::Windows::Main::posX = ( int ) temp.x;
        Settings::UI::Windows::Main::posY = ( int ) temp.y;
        const char* tabs[] = {
                ICON_7,
                ICON_9,
                ICON_8,
                ICON_12
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


