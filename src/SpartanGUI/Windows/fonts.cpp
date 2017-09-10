#include "fonts.h"

/*
bool Fonts::showWindow = false;




void Fonts::RenderWindow() {
    if (!Fonts::showWindow)
        return;
    
        
    ImGui::SetNextWindowSize(ImVec2(185, 270), ImGuiCond_Always);
    if (ImGui::Begin(XORSTR("fonts"), &Fonts::showWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize)) {
        
        if (ImGui::Button(XORSTR("Update"))){
            // shit disabled atm
        }

       


      
        ImGui::PushItemWidth(178);
     //   ImGui::ListBox(XORSTR("##FONTS"), (int *) &Settings::UI::Fonts::font, fonts, IM_ARRAYSIZE(fonts));
           
        ImGui::SliderFloat(XORSTR("##FONTSIZE"), &Settings::UI::Fonts::fontsize, 10, 30,
                                       "Size: %0.f");
        
        ImGui::PopItemWidth();
        
        
        ImGui::End();
    }
}*/