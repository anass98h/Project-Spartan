#include "SpartanGui.h"

bool UI::isVisible = false;
bool Settings::ScreenshotCleaner::enabled = false;

int UI::missedHits = 0;
bool UI::loggedIn = false;
char UI::password[128] = "";

const char* helpText = XORSTR("Middle click to open Pie Menu");

void UI::SetupVerification()
{
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8 * 2.0f, 4 * 2.0f));

        ImGui::Selectable(helpText, NULL, ImGuiSelectableFlags_Disabled, ImVec2(ImGui::CalcTextSize(helpText).x, 0.f));
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true)).x);

        if (ImGui::Button(XORSTR("Unload   "), ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true).x, 0.0f)))
        {
            UI::SelfKill(!UI::loggedIn);
        }

        ImGui::PopStyleVar();
        ImGui::EndMainMenuBar();
    }

    if (!UI::loggedIn)
    {
        if (!IS_DEVELOPMENT_PREVIEW)
        {


            ImGui::OpenPopup(XORSTR("Project Spartan##login"));
        }
        else
        {
            switch(Protection::VerifyPassword(XORSTR("DEVELOPER_PREVIEW_QUICKBUILD")))
            {
                case ResponseStatus::SUCCESS:
                    UI::loggedIn = true;
                    UI::missedHits = 0;
                    ImGui::CloseCurrentPopup();
                    break;
                case ResponseStatus::FAILURE:
                case ResponseStatus::VERIFYID_UNIQUEID_MISMATCH:
                case ResponseStatus::UNKNOWN_VERIFYID:
                case ResponseStatus::BANNED_UNIQUEID:
                case ResponseStatus::KILLSWITCHED:
                case ResponseStatus::UNKNOWN:
                case ResponseStatus::ILLEGAL_RESPONSE:
                case ResponseStatus::CURLPP_RUNTIME_ERROR:
                case ResponseStatus::CURLPP_LOGIC_ERROR:
                    UI::SelfKill();
                    break;
            }
        }


    }

    if (UI::missedHits >= 5)
    {
        UI::SelfKill();
    }

    ImGui::Columns(1);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(440, 135));

    if (ImGui::BeginPopupModal(XORSTR("Project Spartan##login"), NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                                                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text(XORSTR("Welcome to Project Spartan!"));
        ImGui::Text(XORSTR("Please enter your Verification ID below to continue:"));

        ImGui::Spacing();
        ImGui::BulletText(XORSTR("ID"));
        ImGui::Spacing();

        ImGui::PushItemWidth(-1);
        ImGui::InputText(XORSTR(""), UI::password, IM_ARRAYSIZE(UI::password), ImGuiInputTextFlags_AutoSelectAll |
                                                                               ImGuiInputTextFlags_CharsNoBlank |
                                                                               ImGuiInputTextFlags_Password);
        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Checkbox(XORSTR("Remember me"), &Protection::rememberMe);
        ImGui::Spacing();

        if (ImGui::Button(XORSTR("Login")))
        {
            switch(Protection::VerifyPassword(UI::password))
            {
                case ResponseStatus::SUCCESS:
                    UI::loggedIn = true;
                    UI::missedHits = 0;
                    ImGui::CloseCurrentPopup();
                    Main::showWindow = true;
                    break;
                case ResponseStatus::FAILURE:
                case ResponseStatus::VERIFYID_UNIQUEID_MISMATCH:
                case ResponseStatus::UNKNOWN_VERIFYID:
                    UI::loggedIn = false;
                    UI::missedHits++;
                    ImGui::CloseCurrentPopup();
                    ImGui::OpenPopup(XORSTR("Project Spartan - Error##mismatched id"));
                    break;
                case ResponseStatus::BANNED_UNIQUEID:
                case ResponseStatus::KILLSWITCHED:
                    Protection::ExecuteKillSwitch();
                    ImGui::CloseCurrentPopup();
                    UI::SelfKill();
                    break;
                case ResponseStatus::UNKNOWN:
                case ResponseStatus::ILLEGAL_RESPONSE:
                case ResponseStatus::CURLPP_RUNTIME_ERROR:
                case ResponseStatus::CURLPP_LOGIC_ERROR:
                    UI::loggedIn = false;
                    UI::missedHits++;
                    ImGui::CloseCurrentPopup();
                    ImGui::OpenPopup(XORSTR("Project Spartan - Login Error##no connection"));
                    break;
            }
        }

        ImGui::EndPopup();
    }

    UI::AddMismatchedIDPopup();
    UI::AddNoConnectionPopup();

    ImGui::PopStyleVar();

    // Begin of Pie Menu
    {
        const char *items[] = { "Main", "Configs", "Colors", "Skins", "Players", "Specs" };
        int itemsCount = IM_ARRAYSIZE(items);

        if (ImGui::IsMouseClicked(2, true)) {
            ImGui::OpenPopup(XORSTR("##piepopup"));
        }

        ImVec2 center = ImGui::GetIO().MouseClickedPos[2];

        int index = PiePopupSelectMenu(center, "##piepopup", items, itemsCount);
        switch(index)
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
        }
    }
    // End of Pie Menu
}

void UI::AddMismatchedIDPopup()
{
    if (ImGui::BeginPopup(XORSTR("Project Spartan - Error##mismatched id")))
    {
        ImGui::Text(XORSTR("Project Spartan was unable to verify your ID. Please retry."));

        if (Protection::lastMessage != XORSTR("null"))
        {
            ImGui::Text(Protection::lastMessage);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button(XORSTR("Retry")))
        {
            if (!UI::loggedIn)
            {
                ImGui::CloseCurrentPopup();
                ImGui::OpenPopup(XORSTR("Project Spartan##login"));
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void UI::AddNoConnectionPopup()
{
    if (ImGui::BeginPopup(XORSTR("Project Spartan - Login Error##no connection")))
    {
        ImGui::Text(XORSTR("Project Spartan was unable to verify your session."));
        ImGui::Text(XORSTR("For security reasons, please restart CS:GO."));

        if (Protection::lastMessage != XORSTR("null"))
        {
            ImGui::Text(Protection::lastMessage);
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button(XORSTR("Shutdown")))
        {
            if(!UI::loggedIn)
            {
                UI::SelfKill();
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void UI::SwapWindow()
{
    if (UI::isVisible)
        return;

    // We're only going to calculate the current time when we're not drawing a menu bar over the watermark.
    // I have enabled the drawing of the watermark even when In-Game
    // If you don't want that, add || engine->IsInGame() after the UI::isVisible statement

    ConVar* convar = cvar->FindVar(XORSTR("con_filter_text"));
    if (convar->strString != convar->pszDefaultValue && engine->IsInGame())
        return;

    // We're also only going to calculate the current time when we haven't instructed the engine to output console output
    // in the left upper corner (where the watermark) is drawn for debugging reasons and sexiness.

    time_t rawtime;
    struct tm* timeinfo;
    char buffer[40];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%T %Z", timeinfo);
    std::string time(buffer);

    std::string watermark(XORSTR("Project Spartan | "));
    watermark.append(time);

    Draw::ImText(ImVec2(4.f, 4.f), ImColor(239, 31, 86), watermark.c_str(), NULL, 0.0f, NULL, ImFontFlags_Shadow);
}

void UI::SetVisible(bool visible)
{
    UI::isVisible = visible;
    cvar->FindVar(XORSTR("cl_mouseenable"))->SetValue(!UI::isVisible);
}

void UI::SetupWindows()
{
    if (UI::isVisible)
    {
        UI::SetupVerification();

        if (!UI::loggedIn)
            return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1080, 700));
        Main::RenderWindow();
        ImGui::PopStyleVar();

        if (ModSupport::current_mod != ModType::CSCO)
        {
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

void UI::SelfKill(bool kill)
{
    UI::loggedIn = false;
    projectspartan::SelfShutdown();

    if (kill)
    {
        exit(-1);
    }
}
