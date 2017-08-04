#include "Spartangui.h"
static int miss = 0;
bool UI::isVisible = false;
bool ShowMainWindow = false;
bool Settings::ScreenshotCleaner::enabled = false;
bool Settings::UI::oldMenu = false;
bool Settings::UI::otherMenu = false;
bool Settings::UI::combinedMenu = false;
bool toggled = false;
ColorVar Settings::UI::mainColor = ImColor(13, 13, 13, 255);
ColorVar Settings::UI::bodyColor = ImColor(0, 0, 0, 255);
ColorVar Settings::UI::fontColor = ImColor(255, 255, 255, 225);
ColorVar Settings::UI::accentColor = ImColor(244, 66, 83, 255);
bool LoggedIn = false;
static char Pass[256] = "";
std::string data;
std::string contents;

static void ccc() {
    ImGui::CloseCurrentPopup();
    ImGui::OpenPopup(XORSTR("project-spartan.net"));
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



    if (ImGui::Begin("Different Menus", &ShowMainWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize)) {

        if (!LoggedIn)
            ImGui::OpenPopup(XORSTR("project-spartan.net"));

        if (miss >= 4) {
            projectspartan::SelfShutdown();


        }


        ImGui::Columns(1);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(210, 135));


        if (ImGui::BeginPopupModal(XORSTR("project-spartan.net"))) {
            ImGui::Text(XORSTR("You successfully launched project-spartan Private Build please enter your ID to continue "));
            ImGui::Spacing();
            ImGui::BulletText(" Verification-ID ");
            ImGui::Separator();
            ImGui::PushItemWidth(188);
            ImGui::InputText("", Pass, IM_ARRAYSIZE(Pass), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::PopItemWidth();
            ImGui::Separator();
            if (ImGui::Button(XORSTR("Login"))) {
                if (protection::pwmatch(Pass) == true) {

                    LoggedIn = true;
                    ImGui::CloseCurrentPopup();
                    miss = 0;
                    protection::timerstart();
                }
                else {
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

        if (ImGui::Button(XORSTR("Normal Menu  "), ImVec2(ImGui::CalcTextSize(XORSTR("Normal Menu  "), NULL, true).x, 0.0f))) {
            if (!LoggedIn)
                ImGui::OpenPopup(XORSTR("project-spartan.net"));
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
        ImGui::Checkbox("Old Menu", &Settings::UI::oldMenu);
        ImGui::Separator();
        ImGui::Checkbox("Other Menu", &Settings::UI::otherMenu);
        ImGui::Separator();
        ImGui::Checkbox("Combined Menu", &Settings::UI::combinedMenu);
        ImGui::Separator();

        static const char* test_data = "Menu";
        const char* items[] = {"Main", "Config", "Color", "Skins", "pList", "Specs"};
        int items_count = sizeof (items) / sizeof (*items);

        static int selected = -1;

        ImGui::Button(selected >= 0 ? items[selected] : "Pie Menu", ImVec2(150, 30));
        if (ImGui::IsItemActive()) // Don't wait for button release to activate the pie menu
            ImGui::OpenPopup("##piepopup");

        ImVec2 pie_menu_center = ImGui::GetIO().MouseClickedPos[0];
        int n = PiePopupSelectMenu(pie_menu_center, "##piepopup", items, items_count);
        if (n == 1) {
            if (!Configs::showWindow)Configs::showWindow = true;
            else Configs::showWindow = false;
        } else if (n == 0) {
            if (!Main::showWindow)Main::showWindow = true;
            else Main::showWindow = false;
        } else if (n == 2) {
            if (!Colors::showWindow) Colors::showWindow = true;
            else Colors::showWindow = false;
        } else if (n == 3) {
            if (!SkinModelChanger::showWindow)SkinModelChanger::showWindow = true;
            else SkinModelChanger::showWindow = false;
        } else if (n == 4) {
            if (!PlayerList::showWindow)PlayerList::showWindow = true;
            else PlayerList::showWindow = false;
        } else if (n == 5) {
            if (!Settings::ShowSpectators::enabled)Settings::ShowSpectators::enabled = true;
            else Settings::ShowSpectators::enabled = false;
        }


        ImGui::End();
    }


    if (Settings::UI::oldMenu) {

        Settings::UI::otherMenu = false;
        Settings::UI::combinedMenu = false;
        if (!LoggedIn) {
            ImGui::OpenPopup(XORSTR("project-spartan.net"));
        } else {
            if (ImGui::BeginMainMenuBar()) {


                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8 * 2.0f, 4 * 2.0f));

                ImGui::Selectable(XORSTR("Main Window"), &Main::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Main Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                if (ModSupport::current_mod != ModType::CSCO) {
                    ImGui::Selectable(XORSTR("Skin & Model Changer Window"), &SkinModelChanger::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Skin & Model Changer Window"), NULL, true).x, 0.0f));
                    ImGui::SameLine();
                }

                ImGui::Selectable(XORSTR("Config Window"), &Configs::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Config Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                ImGui::Selectable(XORSTR("Spectators Window"), &Settings::ShowSpectators::enabled, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Spectators Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                ImGui::Selectable(XORSTR("Colors Window"), &Colors::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Colors Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();

                ImGui::Selectable(XORSTR("Player List Window"), &PlayerList::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Player List Window"), NULL, true).x, 0.0f));
                ImGui::SameLine();


                ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true)).x);

                if (ImGui::Button(XORSTR("Unload   "), ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true).x, 0.0f))) {
                    projectspartan::SelfShutdown();
                }

                ImGui::PopStyleVar();
                ImGui::EndMainMenuBar();
            }
        }
    } else if (Settings::UI::otherMenu) {

        Settings::UI::oldMenu = false;
        Settings::UI::combinedMenu = false;
        if (!LoggedIn) {
            ImGui::OpenPopup(XORSTR("project-spartan.net"));
        } else {

            ImGui::SetNextWindowSize(ImVec2(150, 200), ImGuiSetCond_FirstUseEver);
            if (ImGui::Begin("OPTIONS", &ShowMainWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize)) {

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
            ImGui::OpenPopup(XORSTR("project-spartan.net"));
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

    Draw::ImDrawText(ImVec2(4.f, 4.f), ImColor(244, 66, 83), watermark.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
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
