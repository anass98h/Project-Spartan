#include "Spartangui.h"

static int miss = 0;
bool UI::isVisible = false;
bool ShowMainWindow = false;
bool Settings::ScreenshotCleaner::enabled = false;
bool Settings::UI::oldMenu = false;
bool Settings::UI::otherMenu = false;
bool Settings::UI::combinedMenu = false;
bool Settings::UI::Pie = false;
bool toggled = false;
ColorVar Settings::UI::mainColor = ImColor(23, 23, 23, 255);
ColorVar Settings::UI::bodyColor = ImColor(0, 0, 0, 220);
ColorVar Settings::UI::fontColor = ImColor(255, 255, 255, 225);
ColorVar Settings::UI::accentColor = ImColor(233, 30, 96, 255);
bool LoggedIn = false;
static char Pass[256] = "";
std::string data;
std::string contents;

// denied

static void ccc() {
    ImGui::CloseCurrentPopup();
    ImGui::OpenPopup(XORSTR("Project Spartan"));
}

static void ppp() {
    if (ImGui::BeginPopup("oops")) {
        ImGui::Text(XORSTR("Oops the ID you entered is wrong please retry"));
        ImGui::Spacing();
        ImGui::Spacing();
        if (ImGui::Button(XORSTR("OK"))) {
            if (!LoggedIn)
                ccc();
        }


        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void SetupMainMenuBar() {


    if (ImGui::Begin("Menu Selector", &ShowMainWindow,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders |
            ImGuiWindowFlags_NoResize)) {

        if (!LoggedIn) {
            if (protection::isVerified()) {
                ImGui::OpenPopup(XORSTR("Project Spartan"));
            } else {
                protection::isVerified();
                ImGui::OpenPopup(XORSTR("Unique ID"));
            }
        }
        if (miss >= 4) {
            projectspartan::SelfShutdown();
            exit(-1);

        }
        if (ImGui::BeginPopupModal(XORSTR("Unique ID"))) {
            ImGui::Text(
                    XORSTR(" Oooops looks like something went wrong \n close this window and open csgo console for more info ")
                    );
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::PushItemWidth(188);

            if (ImGui::Button(XORSTR("Ok"))) {
                UI::SetVisible(!UI::isVisible);
                ImGui::CloseCurrentPopup();

            }


            ImGui::EndPopup();
        }



        ImGui::Columns(1);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(440, 135));


        if (ImGui::BeginPopupModal(XORSTR("Project Spartan"))) {
            ImGui::Text(
                    XORSTR(" Welcome to Project Spartan. Please enter your Verification ID: ")
                    );
            ImGui::Spacing();
            ImGui::BulletText(" Verification-ID ");
            ImGui::Separator();
            ImGui::PushItemWidth(440);
            ImGui::InputText("", Pass, IM_ARRAYSIZE(Pass),
                    ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password |
                    ImGuiInputTextFlags_AutoSelectAll);
            ImGui::PopItemWidth();
            ImGui::Separator();
            if (ImGui::Button(XORSTR("Login"))) {
                if (protection::pwmatch(Pass)) {
                    LoggedIn = true;
                    ImGui::CloseCurrentPopup();
                    miss = 0;
                    protection::timerstart();
                } else {
                    LoggedIn = false;
                    miss += 1;
                    ImGui::CloseCurrentPopup();

                    ImGui::OpenPopup("oops");
                }

            }
            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();


        if (ImGui::Button(XORSTR("Logout  "), ImVec2(ImGui::CalcTextSize(XORSTR("Logout  "), NULL, true).x, 0.0f))) {
            LoggedIn = false;
            Main::showWindow = false;
            UI::isVisible = false;
            Settings::UI::oldMenu = false;
            Settings::UI::otherMenu = false;
            Settings::Aimbot::enabled = false;
            Settings::ESP::enabled = false;
        }
        ImGui::Separator();

        if (ImGui::Button(XORSTR("Normal Menu  "),
                ImVec2(ImGui::CalcTextSize(XORSTR("Normal Menu  "), NULL, true).x, 0.0f))) {
            if (!LoggedIn)
                ImGui::OpenPopup(XORSTR("Project Spartan"));
            if (!toggled && LoggedIn) {
                Main::showWindow = true;
                Settings::UI::otherMenu = false;
                Settings::UI::combinedMenu = false;
                Settings::UI::oldMenu = false;
                toggled = !toggled;
            } else if (toggled && LoggedIn) {
                Main::showWindow = false;
                toggled = !toggled;
            }
        }

        ImGui::Separator();
        if (ImGui::Checkbox(XORSTR("Old Menu"), &Settings::UI::oldMenu)) {
            Settings::UI::otherMenu = false;
            Settings::UI::combinedMenu = false;
        }
        ImGui::Separator();
        if (ImGui::Checkbox(XORSTR("Other Menu"), &Settings::UI::otherMenu)) {
            Settings::UI::oldMenu = false;
            Settings::UI::combinedMenu = false;
        }
        ImGui::Separator();
        if (ImGui::Checkbox(XORSTR("Combined Menu"), &Settings::UI::combinedMenu)) {
            Settings::UI::oldMenu = false;
            Settings::UI::otherMenu = false;
        }
        ImGui::Separator();



        const char* pitems[] = { "Main", "Configs", "Colors", "Skins", "Players", "Specs" };
        int count = sizeof(pitems) / sizeof(*pitems);

        int selectedIndex = -1;

        ImVec2 center = ImGui::GetIO().MouseClickedPos[0];
        int index = PiePopupSelectMenu(center, XORSTR("##piepopup"), pitems, count);
        switch (index)
        {
            case 0:
                Main::showWindow = !Main::showWindow;
                break;
            case 1:
                Configs::showWindow = !Configs::showWindow;
                break;
            case 2:
                Colors::showWindow = !Colors::showWindow;
                break;
            case 3:
                SkinModelChanger::showWindow = !SkinModelChanger::showWindow;
                break;
            case 4:
                PlayerList::showWindow = !PlayerList::showWindow;
                break;
            case 5:
                Settings::ShowSpectators::enabled = !Settings::ShowSpectators::enabled;
                break;
            default:
                //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XORSTR("Somebody fucked up. Invalid index for Pie Menu: %i\n"), index);
                break;
        }

        static const char *test_data = "Menu";
        const char *items[] = {"Main", "Config", "Color", "Skins", "pList", "Specs"};
        int items_count = sizeof (items) / sizeof (*items);

        static int selected = -1;



        ImGui::Button(selected >= 0 ? items[selected] : "Pie Menu", ImVec2(150, 30));
        if (ImGui::IsItemActive()) // Don't wait for button release to activate the pie menu
            ImGui::OpenPopup("##piepopup");



        ImVec2 pie_menu_center = ImGui::GetIO().MouseClickedPos[0];
        int n = PiePopupSelectMenu(pie_menu_center, "##piepopup", items, items_count);
        if (n == 1) {
            Configs::showWindow = !Configs::showWindow;
        } else if (n == 0) {
            Main::showWindow = !Main::showWindow;
        } else if (n == 2) {
            Colors::showWindow = !Colors::showWindow;
        } else if (n == 3) {
            SkinModelChanger::showWindow = !SkinModelChanger::showWindow;
        } else if (n == 4) {
            PlayerList::showWindow = !PlayerList::showWindow;
        } else if (n == 5) {
            Settings::ShowSpectators::enabled = !Settings::ShowSpectators::enabled;
        }

        ImGui::End();



    }
    if (Settings::UI::oldMenu) {

        if (!LoggedIn) {
            if (protection::isVerified()) {
                ImGui::OpenPopup(XORSTR("Project Spartan"));
            } else {
                protection::isVerified();
                ImGui::OpenPopup(XORSTR("Unique ID"));
            }
        } else {
            if (ImGui::BeginMainMenuBar()) {


                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8 * 2.0f, 4 * 2.0f));

                ImGui::Selectable(XORSTR("Main Window"), &Main::showWindow, 0,
                        ImVec2(ImGui::CalcTextSize(XORSTR("Main Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                if (ModSupport::current_mod != ModType::CSCO) {
                    ImGui::Selectable(XORSTR("Skin & Model Changer Window"), &SkinModelChanger::showWindow, 0,
                            ImVec2(ImGui::CalcTextSize(XORSTR("Skin & Model Changer Window"), NULL, true).x,
                            0.0f));
                    ImGui::SameLine();
                }

                ImGui::Selectable(XORSTR("Config Window"), &Configs::showWindow, 0,
                        ImVec2(ImGui::CalcTextSize(XORSTR("Config Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                ImGui::Selectable(XORSTR("Spectators Window"), &Settings::ShowSpectators::enabled, 0,
                        ImVec2(ImGui::CalcTextSize(XORSTR("Spectators Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                ImGui::Selectable(XORSTR("Colors Window"), &Colors::showWindow, 0,
                        ImVec2(ImGui::CalcTextSize(XORSTR("Colors Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                ImGui::Selectable(XORSTR("Player List Window"), &PlayerList::showWindow, 0,
                        ImVec2(ImGui::CalcTextSize(XORSTR("Player List Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();


                ImGui::SameLine(ImGui::GetWindowContentRegionMax().x -
                        ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true)).x);

                if (ImGui::Button(XORSTR("Unload   "),
                        ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true).x, 0.0f))) {
                    projectspartan::SelfShutdown();
                }

                ImGui::PopStyleVar();
                ImGui::EndMainMenuBar();
            }
        }
    } else if (Settings::UI::otherMenu) {


        if (!LoggedIn) {
            if (protection::isVerified()) {
                ImGui::OpenPopup(XORSTR("Project Spartan"));
            } else {
                protection::isVerified();
                ImGui::OpenPopup(XORSTR("Unique ID"));
            }

        } else {

            ImGui::SetNextWindowSize(ImVec2(150, 200), ImGuiSetCond_FirstUseEver);
            if (ImGui::Begin("OPTIONS", &ShowMainWindow,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders |
                    ImGuiWindowFlags_NoResize)) {

                ImGui::Checkbox("Main Window", &Main::showWindow);
                ImGui::Separator();
                ImGui::Checkbox("Spectator Window", &Settings::ShowSpectators::enabled);
                ImGui::Checkbox("PlayerList Window", &PlayerList::showWindow);
                ImGui::Checkbox("Skinchanger Window", &SkinModelChanger::showWindow);
                ImGui::Separator();
                ImGui::Checkbox("Colors Window", &Colors::showWindow);
                ImGui::Checkbox("Config Window", &Configs::showWindow);

                ImGui::End();
            }

        }
    } else if (Settings::UI::combinedMenu) {
        if (!LoggedIn) {
            if (protection::isVerified()) {
                ImGui::OpenPopup(XORSTR("Project Spartan"));
            } else {
                protection::isVerified();
                ImGui::OpenPopup(XORSTR("Unique ID"));
            }
        } else {
            Settings::UI::oldMenu = false;
            Settings::UI::otherMenu = false;
            Main::showWindow = true;

        }
    }
}

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

void UI::SwapWindow() {
    if (UI::isVisible)
        return;

    // We're only going to calculate the current time when we're not drawing a menu bar over the watermark.
    // I have enabled the drawing of the watermark even when In-Game
    // If you don't want that, add || engine->IsInGame() after the UI::isVisible statement

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[40];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof (buffer), "%T %Z", timeinfo);
    std::string time(buffer);

    std::string watermark(XORSTR("Project Spartan | "));
    watermark.append(time);

    Draw::ImText(ImVec2(4.f, 4.f), ImColor(239, 31, 86), watermark.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
}

// This may come in handy if we want to display some good shit in the watermark

std::string OrdinalNumberPrefix(int day) {
    switch (day) {
        case 1:
            return std::string(XORSTR("1st"));
        case 2:
            return std::string(XORSTR("2nd"));
        case 3:
            return std::string(XORSTR("3rd"));
        default:
            std::string format(std::to_string(day));
            return format.append(XORSTR("th"));
    }
}

void UI::SetVisible(bool visible) {
    UI::isVisible = visible;
    cvar->FindVar(XORSTR("cl_mouseenable"))->SetValue(!UI::isVisible);
}

void UI::SetupWindows() {
    if (Settings::UI::oldMenu) {
        if (UI::isVisible) {
            SetupMainMenuBar();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(960, 645));
            Main::RenderWindow();
            ImGui::PopStyleVar();

            if (ModSupport::current_mod != ModType::CSCO) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1050, 645));
                SkinModelChanger::RenderWindow();
                ImGui::PopStyleVar();
            }

            Configs::RenderWindow();
            Colors::RenderWindow();
            PlayerList::RenderWindow();
        }

        ShowSpectators::RenderWindow();
        Radar::RenderWindow();
    } else {
        if (UI::isVisible) {
            SetupMainMenuBar();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1080, 700));
            Main::RenderWindow();
            ImGui::PopStyleVar();

            if (ModSupport::current_mod != ModType::CSCO) {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1050, 645));
                SkinModelChanger::RenderWindow();
                ImGui::PopStyleVar();
            }

            Configs::RenderWindow();
            Colors::RenderWindow();
            PlayerList::RenderWindow();
        }

        ShowSpectators::RenderWindow();
        Radar::RenderWindow();


    }
}

void UI::PieMenu(bool draw)
{
    if(draw)
    {
        ImGui::OpenPopupEx(ImGui::GetID(XORSTR("##piepopup")), false);
    }
    else
    {
        if (ImGui::IsPopupOpen(XORSTR("##piepopup")))
            ImGui::CloseCurrentPopup();
    }
}
