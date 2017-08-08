#include "hvhtab.h"


int Settings::spinFactor::value = 1;

void HvH::RenderTab() {
    const char* yTypes[] = {

        "Legit Trolling", "Legit Trolling 2", "No AA", "Spinbot", "Apostrophe", "Tank", "Tank 2", "Tank 3", "Lby Breaker", "Lby Spin", "Backjitter Random", "CasualJitter", "Lby Jitter", "Jitter", "Backjitter", "Sideways Right", "Sideways Left", "Fake Sideways", "Backwards", "Forwards", "Static", "Static Jitter", "Static small Jitter", "LUA", "LUA2", "CasualAA", // safe
        "LISP", "LISP SIDE", "LISP JITTER", "ANGEL BACKWARDS", "ANGEL INVERSE", "ANGEL SPIN", "LOWERBODY", "LBYONGROUND", "LUA UNCLAMPED", "LUA UNCLAMPED2", // untrusted

    };
    const char* lbyTypes[] = {
        "Breaker 1", "Breaker 2", "Breaker 3", "NONE", 
    };
    const char* zTypes[] = {
        "Reverse", "Autismflip", "TEST",
    };
    const char* xTypes[] = {
        "UP", "FLIP", "DOWN", "DANCE", "FRONT", "LUA", // safe
        "FAKE UP", "FAKE DOWN", "LISP DOWN", "ANGEL DOWN", "ANGEL UP", "LUA UNCLAMPED" // untrusted
    };
    const char* Hugtypes[] = {
        "OFF", "Tux", "PlusDelta", "Apostrophe", "Brute1", "Awoootism"
    };

    ImGui::Columns(2, NULL, true);
    {
        ImGui::BeginChild(XORSTR("HVH1"), ImVec2(0, 0), true);
        {
            ImGui::Text(XORSTR("AntiAim"));
            ImGui::BeginChild(XORSTR("##ANTIAIM"), ImVec2(0, 0), true);
            {
                ImGui::Checkbox(XORSTR("Yaw"), &Settings::AntiAim::Yaw::enabled);
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Yaw Fake"));
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Yaw Actual"));
                    ImGui::Checkbox(XORSTR("Lby Breaker"), &Settings::AntiAim::Lby::enabled);
                    ImGui::Checkbox(XORSTR("Anti Resolver"), &Settings::AntiAim::Yaw::antiResolver);
                    ImGui::Checkbox(XORSTR("Dynamic AA"), &Settings::AntiAim::Yaw::dynamicAA);


                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth(-3);
                    if (ImGui::Combo(XORSTR("##YFAKETYPE"), (int*) & Settings::AntiAim::Yaw::typeFake, yTypes, IM_ARRAYSIZE(yTypes))) {


                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) && Settings::AntiAim::Yaw::typeFake >= AntiAimType_Y::LISP) {
                            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::SPIN;
                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));

                        }

                    }



                    if (ImGui::Combo(XORSTR("##YACTUALTYPE"), (int*) & Settings::AntiAim::Yaw::type, yTypes, IM_ARRAYSIZE(yTypes))) {
                        

                        if (Settings::AntiAim::Yaw::type <= AntiAimType_Y::LEGITTROLLING2) {

                            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;

                        }

                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) && Settings::AntiAim::Yaw::type >= AntiAimType_Y::LISP) {
                            Settings::AntiAim::Yaw::type = AntiAimType_Y::SPIN;
                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));
                        }
                    }
                    if(Settings::AntiAim::Lby::enabled){
                        Settings::AntiAim::Yaw::type = AntiAimType_Y::LBYBREAK;
                    if(ImGui::Combo(XORSTR("##LBYTYPE"), (int*) & Settings::AntiAim::Lby::type, lbyTypes, IM_ARRAYSIZE(lbyTypes))){
                        
                        if (Settings::AntiAim::Lby::type != AntiAimType_LBY::NONE) {
                            Settings::AntiAim::Yaw::type = AntiAimType_Y::LBYBREAK;
                          

                        }
                        if (Settings::AntiAim::Lby::type == AntiAimType_LBY::NONE) {
                            Settings::AntiAim::Lby::enabled = false;

                        }
                    
                    
                        }
                    }    
                    ImGui::PopItemWidth();
                }
                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                ImGui::Checkbox(XORSTR("Roll"), &Settings::AntiAim::Roll::enabled);
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {

                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Roll Actual"));



                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    if (ImGui::Combo(XORSTR("##ZTYPE"), (int*) & Settings::AntiAim::Roll::type, zTypes, IM_ARRAYSIZE(zTypes))) {
                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) && Settings::AntiAim::Roll::type >= AntiAimType_Z::REVERSE) {


                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));
                        }
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::Checkbox(XORSTR("Pitch"), &Settings::AntiAim::Pitch::enabled);
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {
                    ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
                    ImGui::Text(XORSTR("Pitch Actual"));
                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    if (ImGui::Combo(XORSTR("##XTYPE"), (int*) & Settings::AntiAim::Pitch::type, xTypes, IM_ARRAYSIZE(xTypes))) {
                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) && Settings::AntiAim::Pitch::type >= AntiAimType_X::STATIC_UP_FAKE) {
                            Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_UP;
                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));
                        }
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::Text(XORSTR("Disable"));
                ImGui::Separator();
                ImGui::Checkbox(XORSTR("Knife"), &Settings::AntiAim::AutoDisable::knifeHeld);
                ImGui::Checkbox(XORSTR("Bomb"), &Settings::AntiAim::AutoDisable::bombHeld);
                ImGui::Checkbox(XORSTR("No Enemy"), &Settings::AntiAim::AutoDisable::noEnemy);

                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::Text(XORSTR("Edging"));
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {
                    ImGui::Checkbox(XORSTR("Enabled"), &Settings::AntiAim::HeadEdge::enabled);
                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    ImGui::SliderFloat(XORSTR("##EDGEDISTANCE"), &Settings::AntiAim::HeadEdge::distance, 20, 30, "Distance: %0.f");
                    ImGui::PopItemWidth();
                }
                ImGui::Columns(1);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(210, 85));
                if (ImGui::BeginPopupModal(XORSTR("Error###UNTRUSTED_AA"))) {
                    ImGui::Text(XORSTR("You cannot use this antiaim type on a VALVE server."));

                    ImGui::Checkbox(XORSTR("This is not a VALVE server"), &ValveDSCheck::forceUT);

                    if (ImGui::Button(XORSTR("OK")))
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar();

                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
    }
    ImGui::NextColumn();
    {
        ImGui::BeginChild(XORSTR("HVH2"), ImVec2(0, 0), true);
        {   
            ImGui::Text(XORSTR("Resolver"));
            ImGui::Separator();
            ImGui::Checkbox("Resolve All", &Settings::Resolver::resolveAll);
            if (Settings::Resolver::resolveAll)
            { 
            ImGui::Combo("##HUGTYPE", (int*) & Settings::Resolver::Hugtype, Hugtypes, IM_ARRAYSIZE(Hugtypes));
            ImGui::SliderFloat("##HUGTICKS", &Settings::Resolver::ticks, 0, 50, "Ticks: %0.f");
            ImGui::SliderFloat("##HUGMODULO", &Settings::Resolver::modulo, 0, 50, "Modulo: %0.f");
           
            } 
             ImGui::Separator();
            ImGui::Text("Misc");
            ImGui::Checkbox("LBY Indicator", &Settings::lbyindicator::enabled);
            ImGui::Separator();
            ImGui::Text(XORSTR("Movement"));
            ImGui::Checkbox(XORSTR("Auto Crouch"), &Settings::Aimbot::AutoCrouch::enabled);

            ImGui::Separator();
            ImGui::Checkbox(XORSTR("Lua Debug Mode"), &Settings::AntiAim::Lua::debugMode);
            if (Settings::AntiAim::Pitch::type == AntiAimType_X::LUA1 || Settings::AntiAim::Pitch::type == AntiAimType_X::LUA_UNCLAMPED) {
                ImGui::Text(XORSTR("Lua AntiAim Editor -- X Axis"));
                ImGui::InputTextMultiline(XORSTR("##LUAX"), Settings::AntiAim::Lua::scriptX, sizeof (Settings::AntiAim::Lua::scriptX));
            }
            //	ImGui::Separator();

            if (((Settings::AntiAim::Yaw::type == Settings::AntiAim::Yaw::typeFake) && // if they are equal to each other and a LUA type
                    (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA1 ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA2 ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED2))
                    || // OR
                    ((Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA1 && Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA_UNCLAMPED)// Any LUA types that use the same underlying script.
                    || (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA2 && Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA_UNCLAMPED2)
                    || (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED && Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA1)
                    || (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED2 && Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA2)
                    )
                    ) {
                ImGui::Text(XORSTR("Lua AntiAim Editor -- Y Axis(BOTH)"));
                if (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA1 || Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED) {
                    ImGui::InputTextMultiline(XORSTR("##LUAY"), Settings::AntiAim::Lua::scriptY, sizeof (Settings::AntiAim::Lua::scriptY));
                } else {
                    ImGui::InputTextMultiline(XORSTR("##LUAY2"), Settings::AntiAim::Lua::scriptY2, sizeof (Settings::AntiAim::Lua::scriptY2));
                }
            } else {
                if (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA1 || Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y Axis(ACTUAL)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY"), Settings::AntiAim::Lua::scriptY, sizeof (Settings::AntiAim::Lua::scriptY));
                } else if (Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA2 || Settings::AntiAim::Yaw::type == AntiAimType_Y::LUA_UNCLAMPED2) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y2 Axis(ACTUAL)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY2"), Settings::AntiAim::Lua::scriptY2, sizeof (Settings::AntiAim::Lua::scriptY2));
                }
                ImGui::Separator();
                if (Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA1 || Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA_UNCLAMPED) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y Axis(FAKE)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY"), Settings::AntiAim::Lua::scriptY, sizeof (Settings::AntiAim::Lua::scriptY));
                } else if (Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA2 || Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LUA_UNCLAMPED2) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y2 Axis (FAKE)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY2"), Settings::AntiAim::Lua::scriptY2, sizeof (Settings::AntiAim::Lua::scriptY2));
                }
            }


            if (Settings::AntiAim::Yaw::type == AntiAimType_Y::SPIN || Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::SPIN) {
                ImGui::NextColumn();


                {
                    ImGui::BeginChild(XORSTR("HVH3"), ImVec2(0, 0), true);
                    {
                        ImGui::Text(XORSTR("Spin Factor"));

                        {
                            ImGui::PushItemWidth(-1);
                            ImGui::SliderInt(XORSTR("##spinFactor"), &Settings::spinFactor::value, 0, 100, XORSTR("Amount: %1.f"));
                            ImGui::PopItemWidth();

                        }

                        ImGui::EndChild();
                    }


                }
            }
            ImGui::EndChild();
        }

    }
}

