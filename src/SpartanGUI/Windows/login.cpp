#include "login.h"

// static int page = 0; Unused
bool Login::showWindow = false;
// static bool LoggedIn = false; Unused

void Login::RenderWindow() {
    if ( !Login::showWindow )
        return;


    ImGui::SetNextWindowSize( ImVec2( 880, 500 ), ImGuiCond_Always );
    if ( ImGui::Begin( XORSTR( "Login" ), &Login::showWindow,
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ) ) {

        ImGui::Spacing();

        static char Nam[128] = "";
        static char Pass[128] = "";
        ImGui::PushItemWidth( 138 );
        ImGui::Text( " Username " );
        ImGui::InputText( "", Nam, IM_ARRAYSIZE( Nam ) );
        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Text( " Password " );
        ImGui::InputText( "", Pass, IM_ARRAYSIZE( Pass ) );
        ImGui::PopItemWidth();


        ImGui::End();
    }
}
