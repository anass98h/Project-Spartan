#include "main.h"
#include "../../glhook.h"


bool Main::showWindow = false;

void Main::RenderWindow() {
    if (!Main::showWindow)
        return;


    if (Settings::UI::combinedMenu) {
        

        static int page = 0;
        ImGui::SetNextWindowSize(ImVec2(1080, 700), ImGuiSetCond_FirstUseEver);
        if (ImGui::Begin(("project-spartan"), &Main::showWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
 {

            Main::showWindow = true;



            const char* tabs[] = {
                ICON_7,
                ICON_5,
                ICON_9,
                ICON_8,
                ICON_12,

            };





            ImGui::BeginChild("Main1", ImVec2(150, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
            {
                for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
                    int distance = i == page ? 0 : i > page ? i - page : page - i;

                    ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
                            Settings::UI::mainColor.Color().Value.x - (distance * 0.035f),
                            Settings::UI::mainColor.Color().Value.y - (distance * 0.035f),
                            Settings::UI::mainColor.Color().Value.z - (distance * 0.035f),
                            Settings::UI::mainColor.Color().Value.w
                            );

                    ImGui::GetStyle().Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();

                    if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x - 9, ImGui::GetWindowSize().y / IM_ARRAYSIZE(tabs) - 6)))
                        page = i;


                    i < IM_ARRAYSIZE(tabs) - 1;

                }
                ImGui::EndChild();
            }

            ImGui::SameLine();
            ImGui::BeginChild("Main2", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
            {

                switch (page) {
                    case 0:
                        Aimbot::RenderTab();
                        break;
                    case 1:
                        Triggerbot::RenderTab();
                        break;
                    case 2:
                        Visuals::RenderTab();
                        break;
                    case 3:
                       
                            HvH::RenderTab();
                       

                        break;
                    case 4:
                        Misc::RenderTab();
                        break;



                }


                ImGui::EndChild();
            }


            if (ImGui::BeginMenuBar()) {

                if (ImGui::BeginMenu("Menu")) {

                    ImGui::MenuItem("Spectator Window", NULL, &Settings::ShowSpectators::enabled);
                    ImGui::MenuItem("PlayerList Window", NULL, &PlayerList::showWindow);
                    ImGui::MenuItem("Skinchanger Window", NULL, &SkinModelChanger::showWindow);
                    ImGui::MenuItem("Colors Window", NULL, &Colors::showWindow);
                    ImGui::MenuItem("Config Window", NULL, &Configs::showWindow);

                    ImGui::EndMenu();
                }
            }


            ImGui::End();


        }




    }
    else {
        static int page = 0;
        ImGui::SetNextWindowSize(ImVec2(1080, 700), ImGuiSetCond_FirstUseEver);
        if (ImGui::Begin(("project-spartan"), &Main::showWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
 {
            if (Settings::UI::oldMenu || Settings::UI::otherMenu) {
                const char* tabs[] = {
                    ICON_7,
                    ICON_5,
                    ICON_9,
                    ICON_8,
                    ICON_12,

                };









                ImGui::BeginChild("Main1", ImVec2(150, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
                {
                    for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
                        int distance = i == page ? 0 : i > page ? i - page : page - i;

                        ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
                                Settings::UI::mainColor.Color().Value.x - (distance * 0.035f),
                                Settings::UI::mainColor.Color().Value.y - (distance * 0.035f),
                                Settings::UI::mainColor.Color().Value.z - (distance * 0.035f),
                                Settings::UI::mainColor.Color().Value.w
                                );

                        ImGui::GetStyle().Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();

                        if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x - 9, ImGui::GetWindowSize().y / IM_ARRAYSIZE(tabs) - 6)))
                            page = i;


                        i < IM_ARRAYSIZE(tabs) - 1;

                    }
                    ImGui::EndChild();
                }

                ImGui::SameLine();
                ImGui::BeginChild("Main2", ImVec2(0, 0), true, ImGuiWindowFlags_NoResize);
                {

                    switch (page) {
                        case 0:
                            Aimbot::RenderTab();
                            break;
                        case 1:
                            Triggerbot::RenderTab();
                            break;
                        case 2:
                            Visuals::RenderTab();
                            break;
                        case 3:
                          
                                HvH::RenderTab();
                           
                            break;
                        case 4:
                            Misc::RenderTab();
                            break;
                            //   case 5: 
                            //        Others2::RenderTab();


                    }


                    ImGui::EndChild();
                }

                ImGui::End();
            }


            else {
                const char* tabs[] = {
                    ICON_7,
                    ICON_5,
                    ICON_9,
                    ICON_8,
                    ICON_12,
                    ICON_3,
                };



                ImGui::BeginChild("Main1", ImVec2(150, 0), true, ImGuiWindowFlags_NoResize);
                {
                    for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
                        int distance = i == page ? 0 : i > page ? i - page : page - i;

                        ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
                                Settings::UI::mainColor.Color().Value.x - (distance * 0.035f),
                                Settings::UI::mainColor.Color().Value.y - (distance * 0.035f),
                                Settings::UI::mainColor.Color().Value.z - (distance * 0.035f),
                                Settings::UI::mainColor.Color().Value.w
                                );

                        ImGui::GetStyle().Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();

                        if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x - 9, ImGui::GetWindowSize().y / IM_ARRAYSIZE(tabs) - 6)))
                            page = i;


                        i < IM_ARRAYSIZE(tabs) - 1;

                    }
                    ImGui::EndChild();
                }

                ImGui::SameLine();
                ImGui::BeginChild("Main2", ImVec2(0, 0), true, ImGuiWindowFlags_NoResize);
                {



                    switch (page) {
                        case 0:
                            Aimbot::RenderTab();
                            break;
                        case 1:
                            Triggerbot::RenderTab();
                            break;
                        case 2:
                            Visuals::RenderTab();
                            break;
                        case 3:
                           
                                HvH::RenderTab();
                           

                            break;
                        case 4:
                            Misc::RenderTab();
                            break;
                        case 5:
                            Others2::RenderTab();


                    }






                    ImGui::EndChild();
                }

                ImGui::End();
            }

        }
    }
}
