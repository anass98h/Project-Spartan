#include "settingstab.h"

bool SettingsTab::iWantToSeeItAlready = false;

void SettingsTab::RenderTab() {
    ImGui::Columns(1, NULL, true);
    {
        ImGui::Text(XORSTR("Settings"));
        ImGui::Separator();
        ImGui::Text(XORSTR("This is not ready yet."));

        ImGui::Checkbox(XORSTR("But I want to already see it"), &iWantToSeeItAlready);
    }
}
