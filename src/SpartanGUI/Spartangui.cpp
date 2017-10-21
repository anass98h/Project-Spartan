#include "Spartangui.h"
//#include "Windows/fonts.h"

static int miss = 0;
bool UI::isVisible = false;
bool ShowMainWindow = false;
bool Settings::ScreenshotCleaner::enabled = false;
bool Settings::UI::Pie = false;
bool toggled = false;
ColorVar Settings::UI::mainColor = ImColor( 13, 13, 13, 246 );      //    |-»
ColorVar Settings::UI::bodyColor = ImColor( 13, 13, 13,
                                            246 );      //    |-»   these new colors look p and the old ones where shit,
ColorVar Settings::UI::fontColor = ImColor( 90, 178, 255, 255 );    //    |-»   get over it /savage
ColorVar Settings::UI::accentColor = ImColor( 43, 115, 178, 74 );  //    |-»
bool LoggedIn = false;
static char Pass[256] = "";
std::string data;
std::string contents;
bool Settings::UI::middle = true;
bool Settings::UI::right = false;
ImVec2 center = { 100, 100 };
/* Unused function
static void ccc() {
    ImGui::CloseCurrentPopup();
    ImGui::OpenPopup( XORSTR( "Project Spartan" ) );
}
*/
/* Unused function
static void ppp() {
    if ( ImGui::BeginPopup( "oops" ) ) {
        ImGui::Text( XORSTR( "Oops the ID you entered is wrong please retry" ) );
        ImGui::Spacing();
        ImGui::Spacing();
        if ( ImGui::Button( XORSTR( "OK" ) ) ) {
            if ( !LoggedIn )
                ccc();
        }


        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}
*/
void SetupMainMenuBar() {


    if ( ImGui::Begin( "Logout", &ShowMainWindow, ImGuiWindowFlags_AlwaysAutoResize |
                                                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                  ImGuiWindowFlags_NoTitleBar ) ) {

        if ( !LoggedIn ) {
            if ( protection::isVerified() ) {
                ImGui::OpenPopup( XORSTR( "Project Spartan" ) );
            } else {
                protection::isVerified();
                ImGui::OpenPopup( XORSTR( "Unique ID" ) );
            }
        }
        if ( miss >= 4 ) {
            projectspartan::SelfShutdown();


        }
        if ( ImGui::BeginPopupModal( XORSTR( "Unique ID" ) ) ) {
            ImGui::Text(
                    XORSTR( " Oooops looks like something went wrong \n close this window and open csgo console for more info " )
            );
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::PushItemWidth( 188 );

            if ( ImGui::Button( XORSTR( "Ok" ) ) ) {
                UI::SetVisible( !UI::isVisible );
                ImGui::CloseCurrentPopup();

            }


            ImGui::EndPopup();
        }


        ImGui::Columns( 1 );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 440, 170 ) );


        if ( ImGui::BeginPopupModal( XORSTR( "Project Spartan" ), NULL, ImGuiWindowFlags_NoResize ) ) {
            ImGui::Text(
                    XORSTR( "Welcome to Project Spartan. \nPlease enter your Verification ID: " )
            );
            ImGui::SameLine();
            if ( ImGui::Button( " ", ImVec2( 1, 1 ) ) ) {
                LoggedIn = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::Spacing();
            ImGui::BulletText( " Verification-ID " );
            ImGui::Separator();
            ImGui::PushItemWidth( 440 );
            ImGui::InputText( "", Pass, IM_ARRAYSIZE( Pass ),
                              ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password |
                              ImGuiInputTextFlags_AutoSelectAll );
            ImGui::PopItemWidth();
            ImGui::Separator();
            if ( ImGui::Button( XORSTR( "Login" ) ) ) {
                if ( protection::pwmatch( Pass ) ) {
                    LoggedIn = true;
                    ImGui::CloseCurrentPopup();
                    miss = 0;
                    protection::timerstart();
                } else {
                    LoggedIn = false;
                    miss += 1;
                    ImGui::CloseCurrentPopup();

                    ImGui::OpenPopup( "whoops" );
                }

            }
            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();

        if ( ImGui::Button( XORSTR( "Logout  " ),
                            ImVec2( ImGui::CalcTextSize( XORSTR( "Logout  " ), NULL, true ).x, 0.0f ) ) ) {
            LoggedIn = false;
            Main::showWindow = false;
            SkinModelChanger::showWindow = false;
            Colors::showWindow = false;
            PlayerList::showWindow = false;
            Configs::showWindow = false;

            UI::isVisible = false;
            Settings::Aimbot::enabled = false;
            Settings::ESP::enabled = false;
        }

        ImGui::Separator();
        if ( ImGui::Button( XORSTR( "RQUIT  " ),
                            ImVec2( ImGui::CalcTextSize( XORSTR( "RQUIT  " ), NULL, true ).x, 0.0f ) ) )
            projectspartan::SelfShutdown();

        ImGui::Separator();
        if ( ImGui::Checkbox( "Right click ", &Settings::UI::right ) )
            Settings::UI::middle = false;
        ImGui::Separator();
        if ( ImGui::Checkbox( "Middle click ", &Settings::UI::middle ) )
            Settings::UI::right = false;

        const char* items[] = { "Main", "Config", "Color", "Skins", "pList", "Specs", "Resolver" };
        int items_count = sizeof( items ) / sizeof( *items );


        if ( Settings::UI::middle ) {
            if ( ImGui::IsMouseClicked( 2, true ) )
                ImGui::OpenPopup( "##piepopup" );

            center = ImGui::GetIO().MouseClickedPos[2];
        } else if ( Settings::UI::right ) {

            if ( ImGui::IsMouseClicked( 1, true ) )
                ImGui::OpenPopup( "##piepopup" );

            center = ImGui::GetIO().MouseClickedPos[1];
        }

        int n = PiePopupSelectMenu( center, "##piepopup", items, items_count );
        switch ( n ) { // a switch for my love Myrrib


            case 1: {
                Configs::showWindow = !Configs::showWindow;
                break;
            }
            case 0: {
                Main::showWindow = !Main::showWindow;
                break;
            }
            case 2: {
                Colors::showWindow = !Colors::showWindow;
                break;
            }
            case 3: {
                SkinModelChanger::showWindow = !SkinModelChanger::showWindow;
                break;
            }
            case 4: {
                PlayerList::showWindow = !PlayerList::showWindow;
                break;
            }
            case 5: {
                Settings::ShowSpectators::enabled = !Settings::ShowSpectators::enabled;
                break;
            }
            case 6: {
                ResolverInfo::showWindow = !ResolverInfo::showWindow;
            }
        }


        ImGui::End();


    }


}

#define IM_ARRAYSIZE( _ARR )  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

void UI::SwapWindow() {
    if ( UI::isVisible || ( engine->IsInGame() && ( Settings::EventLogger::enabled /*&& EventLogger::events.size() > 0*/ ) ) )
        return;

    // We're only going to calculate the current time when we're not drawing a menu bar over the watermark.
    // I have enabled the drawing of the watermark even when In-Game
    // If you don't want that, add || engine->IsInGame() after the UI::isVisible statement

    time_t rawtime;
    struct tm* timeinfo;
    char buffer[40];

    time( &rawtime );
    timeinfo = localtime( &rawtime );
    strftime( buffer, sizeof( buffer ), "%T %Z", timeinfo );
    std::string time( buffer );

    std::string watermark( XORSTR( "Project Spartan | " ) );
    watermark.append( time );

    Draw::ImText( ImVec2( 4.f, 4.f ), ImColor( 26, 104, 173 ), watermark.c_str(), NULL, 0.0f, NULL,
                  ImFontFlags_Shadow );
}

// This may come in handy if we want to display some good shit in the watermark

std::string OrdinalNumberPrefix( int day ) {
    switch ( day ) {
        case 1:
            return std::string( XORSTR( "1st" ) );
        case 2:
            return std::string( XORSTR( "2nd" ) );
        case 3:
            return std::string( XORSTR( "3rd" ) );
        default:
            std::string format( std::to_string( day ) );
            return format.append( XORSTR( "th" ) );
    }
}

void UI::SetVisible( bool visible ) {
    UI::isVisible = visible;
    cvar->FindVar( XORSTR( "cl_mouseenable" ) )->SetValue( !UI::isVisible );
}

void UI::SetupWindows() {


    if ( UI::isVisible ) {
        SetupMainMenuBar();

        ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 1080, 700 ) );
        Main::RenderWindow();
        ImGui::PopStyleVar();


        if ( ModSupport::current_mod != ModType::CSCO ) {
            ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 1050, 645 ) );
            SkinModelChanger::RenderWindow();
            ImGui::PopStyleVar();
        }

        Configs::RenderWindow();
        Colors::RenderWindow();
        PlayerList::RenderWindow();
        ResolverInfo::RenderWindow();
        //         Fonts::RenderWindow();
    }

    ShowSpectators::RenderWindow();
    Radar::RenderWindow();


}
