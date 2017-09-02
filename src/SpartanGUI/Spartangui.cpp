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
ColorVar Settings::UI::mainColor = ImColor(13, 13, 13, 255);
ColorVar Settings::UI::bodyColor = ImColor(0, 0, 0, 255);
ColorVar Settings::UI::fontColor = ImColor(255, 255, 255, 225);
ColorVar Settings::UI::accentColor = ImColor(244, 66, 83, 255);
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


    if (ImGui::Begin("Logout", &ShowMainWindow, ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

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
            SkinModelChanger::showWindow = false;
            Colors::showWindow = false;
            PlayerList::showWindow = false;
            Configs::showWindow = false;

            UI::isVisible = false;
            Settings::UI::oldMenu = false;
            Settings::UI::otherMenu = false;
            Settings::Aimbot::enabled = false;
            Settings::ESP::enabled = false;
        }

        ImGui::Separator();
        if (ImGui::Button(XORSTR("RQUIT  "), ImVec2(ImGui::CalcTextSize(XORSTR("RQUIT  "), NULL, true).x, 0.0f)))
            projectspartan::SelfShutdown();



        const char *items[] = {"Main", "Config", "Color", "Skins", "pList", "Specs"};
        int items_count = sizeof (items) / sizeof (*items);





        if (ImGui::IsMouseClicked(1, true))
            ImGui::OpenPopup("##piepopup");

        ImVec2 center = ImGui::GetIO().MouseClickedPos[1];
        int n = PiePopupSelectMenu(center, "##piepopup", items, items_count);
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



}

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

void UI::SwapWindow() {
    if (UI::isVisible) {
        Draw::ImText(ImVec2(4.f, 4.f), ImColor(0, 180, 12), "Spartan-menu active", NULL, 0.0f, NULL, ImFontFlags_Outline);
        return;
    }
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
