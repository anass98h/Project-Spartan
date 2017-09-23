#include "mischvhtab.h"

const char* hugTypes[] = {

        "OFF", "Rasp", "Brute Log EAP", "Bruteforce", "TapTipTopTupTep", "Myrrib's DELTA", "Myrrib Old", "LBY Backtrack"

};

void MiscHvHTab::RenderTab() {
    ImGui::Columns( 2, NULL, true );
    {
        ImGui::BeginChild( XORSTR( "##HVHMISC1" ), ImVec2( 0, 0 ), true );
        {
            ImGui::Text( XORSTR( "Options" ) );
            ImGui::BeginChild( XORSTR( "##HVHMISC3" ), ImVec2( 0, 0 ), true );
            {
                ImGui::Checkbox( XORSTR( "Resolver" ), &Settings::Resolver::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Type" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Ticks" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Modulo" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -3 );
                        ImGui::Combo( XORSTR( "##HUGTYPE" ), ( int* ) &Settings::Resolver::Hugtype,
                                      hugTypes, IM_ARRAYSIZE( hugTypes ) );
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##HUGTICKS" ), &Settings::Resolver::ticks, 0, 50,
                                            XORSTR( "%.1f" ) );
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##HUGMODULO" ), &Settings::Resolver::modulo, 0, 50,
                                            XORSTR( "%.1f" ) );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::Checkbox( XORSTR( "Lag Compensation" ), &Settings::Resolver::LagComp );
                        SetTooltip( XORSTR( "WARNING: Needs testing." ) );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::Checkbox( XORSTR( "Resolve Pitch" ), &Settings::Resolver::pitch );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Text( XORSTR( "Disable" ) );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::Checkbox( XORSTR( "Knife" ), &Settings::AntiAim::Misc::AutoDisable::knifeHeld );
                        ImGui::Checkbox( XORSTR( "Bomb" ), &Settings::AntiAim::Misc::AutoDisable::bombHeld );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::Checkbox( XORSTR( "No Enemy" ), &Settings::AntiAim::Misc::AutoDisable::noEnemy );
                        ImGui::Checkbox( XORSTR( "Freeze Time" ), &Settings::AntiAim::Misc::AutoDisable::freezeTime );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Text( XORSTR( "Indicators" ) );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::Checkbox( XORSTR( "Angles" ), &Settings::AngleIndicator::enabled );
                        ImGui::Checkbox( XORSTR( "Velocity" ), &Settings::AngleIndicator::Veloc );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::Checkbox( XORSTR( "Lower Body Yaw" ), &Settings::lbyindicator::enabled );
                        ImGui::Checkbox( XORSTR( "Car" ), &Settings::SpeedIndicator::enabled );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Text( XORSTR( "Miscellaneous" ) );
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Auto Crouch" ), &Settings::Aimbot::AutoCrouch::enabled );

                ImGui::Separator();
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }

    }
    ImGui::NextColumn();
    {
        ImGui::BeginChild( XORSTR( "##HVHMISC2" ), ImVec2( 0, 0 ), true );
        {
            ImGui::Text( XORSTR( "Key Bindings" ) );
            ImGui::BeginChild( XORSTR( "##HVHMISC4" ), ImVec2( 0, 0 ), true );
            {
                ImGui::Text( XORSTR( "Movement" ) );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::Checkbox( XORSTR( "Fakewalk - AIMWARE" ), &Settings::FakewalkAW::enabled );
                        ImGui::Checkbox( XORSTR( "Fakewalk" ), &Settings::Fakewalk::enabled );
                        ImGui::Checkbox( XORSTR( "Choke Packets" ), &Settings::ChokePackets::enabled );
                        ImGui::Checkbox( XORSTR( "Circle Strafer" ), &Settings::CircleStrafe::enabled );
                    }
                    ImGui::NextColumn();
                    {
                        UI::KeyBindButton( &Settings::FakewalkAW::key );
                        UI::KeyBindButton( &Settings::Fakewalk::key );
                        UI::KeyBindButton( &Settings::ChokePackets::key );
                        UI::KeyBindButton( &Settings::CircleStrafe::key );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Text( XORSTR( "Miscellaneous" ) );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::Checkbox( XORSTR( "Slow Motion" ), &Settings::SlowMo::enabled );
                        SetTooltip( XORSTR( "WARNING: This feature is not working as intended." ) );
                    }
                    ImGui::NextColumn();
                    {
                        UI::KeyBindButton( &Settings::SlowMo::key );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Text( XORSTR( "Console" ) );
                {
                    ImGui::Separator();

                    // TODO:
                    // Implement a console in which it is possible to bind keys
                    // like AimWare. e.g: bind m "rbot_aa_yaw_real_add 90"

                }
                ImGui::Separator();

                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
    }
}
