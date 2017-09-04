#include "hvhtab.h"

int Settings::spinFactor::value = 1;
int Settings::customYaw::value = 1;
int Settings::customYaw2::value = 1;
bool Settings::customYaw::lby;
bool Settings::customYaw2::lby;
void HvH::RenderTab() {

    const char *yTypes[] = {
            "no aa", "Backwards", "Forwards", "FakeSideWays", "Spin", "Backjitter", "FJitter", "Tank1", "Tank2", "richi's eap", "myrrib's eap",
            "lbyBreaker", "FakeLBY", "lbySpin", "LBYjitter", "LBY", "LEGITTROLLING", "LEGITTROLLING2", "Custom", "Custom2" // safe
    };

    const char *lbyTypes[] = {
            "Breaker 1", "Breaker 2", "Breaker 3","Avoz", "NONE"
    };

    const char *zTypes[] = {
            "TEST", // not so untrusted
    };

    const char *xTypes[] = {
            "up", "flip", "down",
    };

    const char *Hugtypes[] = {
            "OFF", "Tux", "Plus Delta", "PasteHub", "Brute Log EAP", "Bruteforce", "TapTipTopTupTep", "I'm feeling Lucky", "Myrrib's DELTA"
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
                    if (ImGui::Combo(XORSTR("##YFAKETYPE"), (int *) &Settings::AntiAim::Yaw::typeFake, yTypes,
                                     IM_ARRAYSIZE(yTypes))) {
                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) &&
                            !Settings::AntiAim::allowUntrustedAngles &&
                            Settings::AntiAim::Yaw::typeFake >= AntiAimType_Y::CUSTOM) {
                            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::SPIN;

                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));
                        }
                    }
                    if (ImGui::Combo(XORSTR("##YACTUALTYPE"), (int *) &Settings::AntiAim::Yaw::type, yTypes,
                                     IM_ARRAYSIZE(yTypes))) {
                        if (Settings::AntiAim::Yaw::type <= AntiAimType_Y::LEGITTROLLING2) {
                            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
                        }
                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) &&
                            !Settings::AntiAim::allowUntrustedAngles &&
                            Settings::AntiAim::Yaw::type >= AntiAimType_Y::CUSTOM) {
                            Settings::AntiAim::Yaw::type = AntiAimType_Y::SPIN;
                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));
                        }
                    }
                    if (Settings::AntiAim::Lby::enabled) {
                        if (ImGui::Combo(XORSTR("##LBYTYPE"), (int *) &Settings::AntiAim::Lby::type, lbyTypes,
                                         IM_ARRAYSIZE(lbyTypes))) {
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
                ImGui::Text(XORSTR("Roll Actual"));
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {
                    ImGui::Checkbox(XORSTR("Roll"), &Settings::AntiAim::Roll::enabled);

                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth(-1);
                    if (ImGui::Combo(XORSTR("##ZTYPE"), (int *) &Settings::AntiAim::Roll::type, zTypes,
                                     IM_ARRAYSIZE(zTypes))) {
                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) &&
                            !Settings::AntiAim::allowUntrustedAngles &&
                            Settings::AntiAim::Roll::type >= AntiAimType_Z::TEST) {


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
                    if (ImGui::Combo(XORSTR("##XTYPE"), (int *) &Settings::AntiAim::Pitch::type, xTypes,
                                     IM_ARRAYSIZE(xTypes))) {
                        if (!ValveDSCheck::forceUT && ((*csGameRules) && (*csGameRules)->IsValveDS()) &&
                            !Settings::AntiAim::allowUntrustedAngles &&
                            Settings::AntiAim::Pitch::type >= AntiAimType_X::STATIC_UP) {
                            Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_UP;
                            ImGui::OpenPopup(XORSTR("Error###UNTRUSTED_AA"));
                        }
                    }
                    ImGui::PopItemWidth();
                }

                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::Text(XORSTR("Switch AA"));
                ImGui::Separator();
                ImGui::Columns(2, NULL, true);
                {
                    ImGui::Checkbox(XORSTR("Enabled"), &Settings::AntiAim::SwitchAA::enabled);
                }
                ImGui::NextColumn();
                {
                    UI::KeyBindButton(&Settings::AntiAim::SwitchAA::key);
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
                    ImGui::SliderFloat(XORSTR("##EDGEDISTANCE"), &Settings::AntiAim::HeadEdge::distance, 20, 30,
                                       "Distance: %0.f");
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
            ImGui::Checkbox(XORSTR("Enable"), &Settings::Resolver::enabled);
            if (Settings::Resolver::enabled) {
                ImGui::Combo(XORSTR("##HUGTYPE"), (int *) &Settings::Resolver::Hugtype, Hugtypes, IM_ARRAYSIZE(Hugtypes));
                ImGui::SliderFloat(XORSTR("##HUGTICKS"), &Settings::Resolver::ticks, 0, 50, XORSTR("Ticks: %0.f"));
                ImGui::SliderFloat(XORSTR("##HUGMODULO"), &Settings::Resolver::modulo, 0, 50, XORSTR("Modulo: %0.f"));
                ImGui::Checkbox(XORSTR("crash game"), &Settings::Resolver::LagComp);
            }

            ImGui::Separator();

            ImGui::Text(XORSTR("Indicators"));

            ImGui::Checkbox(XORSTR("Lower Body Yaw"), &Settings::lbyindicator::enabled);

            ImGui::Checkbox(XORSTR("Angle"), &Settings::AngleIndicator::enabled);

            ImGui::Checkbox(XORSTR("Velocity"), &Settings::AngleIndicator::Veloc);


            ImGui::Text(XORSTR("Movement"));
            ImGui::Separator();
            ImGui::Columns(2, NULL, true);
            {
                ImGui::Checkbox(XORSTR("Fakewalk"), &Settings::Fakewalk::enabled);
                ImGui::Checkbox(XORSTR("CircleStrafe"), &Settings::CircleStrafe::enabled);
                ImGui::Checkbox(XORSTR("Auto Crouch"), &Settings::Aimbot::AutoCrouch::enabled);
            }
            ImGui::NextColumn();
            {
                UI::KeyBindButton(&Settings::Fakewalk::key);
                UI::KeyBindButton(&Settings::CircleStrafe::key);
            }


            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::Text(XORSTR("Miscellaneous"));
            ImGui::Separator();
            ImGui::Columns(2, NULL, true);
            {
                ImGui::Checkbox(XORSTR("Slowmotion"), &Settings::SlowMo::enabled); // why you move my memes ? :feelssad:
                ImGui::Checkbox(XORSTR("Allow Untrusted Angles"), &Settings::AntiAim::allowUntrustedAngles);
            }
            ImGui::NextColumn();
            {
                UI::KeyBindButton(&Settings::SlowMo::key);
            }
            ImGui::Columns(1);
            ImGui::Separator();

            ImGui::Separator();

            ImGui::Checkbox(XORSTR("Lua Debug Mode"), &Settings::AntiAim::Lua::debugMode);
            if (Settings::AntiAim::Pitch::type == AntiAimType_X::STATIC_UP ||
                Settings::AntiAim::Pitch::type == AntiAimType_X::STATIC_UP) {
                ImGui::Text(XORSTR("Lua AntiAim Editor -- X Axis"));
                ImGui::InputTextMultiline(XORSTR("##LUAX"), Settings::AntiAim::Lua::scriptX,
                                          sizeof(Settings::AntiAim::Lua::scriptX));
            }
            //	ImGui::Separator();

            if (((Settings::AntiAim::Yaw::type == Settings::AntiAim::Yaw::typeFake) &&
                 // if they are equal to each other and a LUA type
                 (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                  Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                  Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM2 ||
                  Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM2))
                || // OR
                ((Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM2 && Settings::AntiAim::Yaw::typeFake ==
                                                                         AntiAimType_Y::CUSTOM2)// Any LUA types that use the same underlying script.
                 || (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM &&
                     Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM)
                 || (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM &&
                     Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM)
                 || (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM &&
                     Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM)
                )
                    ) {
                ImGui::Text(XORSTR("Lua AntiAim Editor -- Y Axis(BOTH)"));
                if (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM) {
                    ImGui::InputTextMultiline(XORSTR("##LUAY"), Settings::AntiAim::Lua::scriptY,
                                              sizeof(Settings::AntiAim::Lua::scriptY));
                } else {
                    ImGui::InputTextMultiline(XORSTR("##LUAY2"), Settings::AntiAim::Lua::scriptY2,
                                              sizeof(Settings::AntiAim::Lua::scriptY2));
                }
            } else {
                if (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y Axis(ACTUAL)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY"), Settings::AntiAim::Lua::scriptY,
                                              sizeof(Settings::AntiAim::Lua::scriptY));
                } else if (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                           Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y2 Axis(ACTUAL)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY2"), Settings::AntiAim::Lua::scriptY2,
                                              sizeof(Settings::AntiAim::Lua::scriptY2));
                }
                ImGui::Separator();
                if (Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM ||
                    Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y Axis(FAKE)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY"), Settings::AntiAim::Lua::scriptY,
                                              sizeof(Settings::AntiAim::Lua::scriptY));
                } else if (Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM ||
                           Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM) {
                    ImGui::Text(XORSTR("Lua AntiAim Editor -- Y2 Axis (FAKE)"));
                    ImGui::InputTextMultiline(XORSTR("##LUAY2"), Settings::AntiAim::Lua::scriptY2,
                                              sizeof(Settings::AntiAim::Lua::scriptY2));
                }
            }

            ImGui::NextColumn();
            {

                if (Settings::AntiAim::Yaw::type == AntiAimType_Y::SPIN ||
                    Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::SPIN ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                    Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM2 ||
                    Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM2 ||
                    Settings::AntiAim::Yaw::type == AntiAimType_Y::LBYSPIN ||
                    Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LBYSPIN) {
                    ImGui::BeginChild(XORSTR("HVH3"), ImVec2(0, 0), true);
                    {
                        if (Settings::AntiAim::Yaw::type == AntiAimType_Y::SPIN ||
                            Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::SPIN ||
                            Settings::AntiAim::Yaw::type == AntiAimType_Y::LBYSPIN ||
                            Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::LBYSPIN) {
                            ImGui::Text(XORSTR("Spin Factor"));

                            {
                                ImGui::PushItemWidth(-1);
                                ImGui::SliderInt(XORSTR("##spinFactor"), &Settings::spinFactor::value, 0, 100,
                                                 XORSTR("Amount: %1.f"));
                                ImGui::PopItemWidth();

                            }
                        }
                        ImGui::Separator();
                        if (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM ||
                            Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM) {
                            ImGui::Text(XORSTR("Custom Yaw"));

                            {
                                ImGui::PushItemWidth(-1);
                                ImGui::Checkbox(XORSTR("Use LBY"), &Settings::customYaw::lby);
                                ImGui::SliderInt(XORSTR("##customYaw"), &Settings::customYaw::value,
                                                 !Settings::AntiAim::allowUntrustedAngles ? -180 : -10000,
                                                 !Settings::AntiAim::allowUntrustedAngles ? 180 : 10000,
                                                 XORSTR("Amount: %1.f"));
                                ImGui::PopItemWidth();

                            }
                        }
                        ImGui::Separator();
                        if (Settings::AntiAim::Yaw::type == AntiAimType_Y::CUSTOM2 ||
                            Settings::AntiAim::Yaw::typeFake == AntiAimType_Y::CUSTOM2) {
                            ImGui::Text(XORSTR("Custom Yaw 2"));
                            {
                                ImGui::PushItemWidth(-1);
                                ImGui::Checkbox(XORSTR("Use LBY"), &Settings::customYaw2::lby);
                                ImGui::SliderInt(XORSTR("##customYaw2"), &Settings::customYaw2::value,
                                                 !Settings::AntiAim::allowUntrustedAngles ? -180 : -10000,
                                                 !Settings::AntiAim::allowUntrustedAngles ? 180 : 10000,
                                                 XORSTR("Amount: %1.f"));
                                ImGui::PopItemWidth();

                            }
                        }

                        ImGui::Separator();

                        ImGui::EndChild();
                    }
                }

            }
            ImGui::EndChild();
        }

    }
}