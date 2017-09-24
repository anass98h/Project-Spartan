#include "movinghvhtab.h"

void Moving::RenderTab() {
    ImGui::Columns( 2, NULL, true );
    {
        ImGui::BeginChild( XORSTR( "##ANTIAIM1" ), ImVec2( 0, 0 ), true );
        {
            ImGui::Text( XORSTR( "Anti-Aim" ) );
            ImGui::BeginChild( XORSTR( "##ANTIAIM3" ), ImVec2( 0, 0 ), true );
            {
                ImGui::Checkbox( XORSTR( "Yaw" ), &Settings::AntiAim::Moving::Yaw::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        {
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                            ImGui::Text( XORSTR( "Fake Yaw" ) );
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                            ImGui::Text( XORSTR( "Fake Yaw Add" ) );
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        }
                        //ImGui::Separator();
                        {
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                            ImGui::Text( XORSTR( "Real Yaw" ) );
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                            ImGui::Text( XORSTR( "Real Yaw Add" ) );
                            ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        }
                    }
                    ImGui::NextColumn();
                    {
                        {
                            ImGui::PushItemWidth( -3 );
                            ImGui::Combo( XORSTR( "##YFAKE" ), ( int* ) &Settings::AntiAim::Moving::Yaw::typeFake,
                                          HvH::yTypes, IM_ARRAYSIZE( HvH::yTypes ) );
                            ImGui::PopItemWidth();
                            ImGui::PushItemWidth( -1 );
                            ImGui::SliderFloat( XORSTR( "##YFAKECUSTOM" ), &Settings::AntiAim::Moving::Yaw::typeFakeAdd,
                                                Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
                                                Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
                                                XORSTR( Settings::AntiAim::Moving::Yaw::typeFakeAdd > 0 ? XORSTR(
                                                        "+%.1f" )
                                                                                                        : XORSTR(
                                                                "%.1f" ) ) );
                            ImGui::PopItemWidth();
                        }
                        ImGui::Separator();
                        {
                            ImGui::PushItemWidth( -3 );
                            ImGui::Combo( XORSTR( "##YREAL" ), ( int* ) &Settings::AntiAim::Moving::Yaw::type,
                                          HvH::yTypes, IM_ARRAYSIZE( HvH::yTypes ) );
                            ImGui::PopItemWidth();
                            ImGui::PushItemWidth( -1 );
                            ImGui::SliderFloat( XORSTR( "##YCUSTOM" ), &Settings::AntiAim::Moving::Yaw::typeAdd,
                                                Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
                                                Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
                                                XORSTR( Settings::AntiAim::Moving::Yaw::typeAdd > 0 ? XORSTR( "+%.1f" )
                                                                                                    : XORSTR(
                                                                "%.1f" ) ) );
                            ImGui::PopItemWidth();
                        }
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Pitch" ), &Settings::AntiAim::Moving::Pitch::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Real Pitch" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Custom Pitch" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -3 );
                        ImGui::Combo( XORSTR( "##XREAL" ), ( int* ) &Settings::AntiAim::Moving::Pitch::type,
                                      HvH::xTypes, IM_ARRAYSIZE( HvH::xTypes ) );
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##XCUSTOM" ), &Settings::AntiAim::Moving::Pitch::custom,
                                            Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
                                            Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
                                            XORSTR( Settings::AntiAim::Moving::Pitch::custom > 0 ? XORSTR( "+%.1f" )
                                                                                                 : XORSTR( "%.1f" ) ) );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Roll" ), &Settings::AntiAim::Moving::Roll::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Real Roll" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -3 );
                        ImGui::Combo( XORSTR( "##ZREAL" ), ( int* ) &Settings::AntiAim::Moving::Roll::type,
                                      HvH::zTypes, IM_ARRAYSIZE( HvH::zTypes ) );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Switch Anti-Aim" ), &Settings::AntiAim::Moving::SwitchAA::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Key" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        UI::KeyBindButton( &Settings::AntiAim::Moving::SwitchAA::key );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "LBY Breaker" ), &Settings::AntiAim::Moving::LBY::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Breaker" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -3 );
                        ImGui::Combo( XORSTR( "##LBYREAL" ), ( int* ) &Settings::AntiAim::Moving::LBY::type,
                                      HvH::lbyTypes, IM_ARRAYSIZE( HvH::lbyTypes ) );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Head Edge" ), &Settings::AntiAim::Moving::HeadEdge::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Distance" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##EDGEDISTANCE" ), &Settings::AntiAim::Moving::HeadEdge::distance,
                                            20, 30,
                                            "%0.f" );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
    }
    ImGui::NextColumn();
    {
        ImGui::BeginChild( XORSTR( "##ANTIAIM2" ), ImVec2( 0, 0 ), true );
        {
            ImGui::Text( XORSTR( "Options" ) );
            ImGui::BeginChild( XORSTR( "##AAOPTIONS" ), ImVec2( 0, 0 ), true );
            {
                ImGui::Checkbox( XORSTR( "Anti Resolver" ), &Settings::AntiAim::Moving::antiResolver );
                ImGui::Separator();
                ImGui::Checkbox( XORSTR( "Dynamic Anti-Aim" ), &Settings::AntiAim::Moving::dynamicAA );
                ImGui::Separator();
                ImGui::Checkbox( XORSTR( "Untrusted angles" ), &Settings::AntiAim::Moving::untrustedAngles );
                ImGui::Separator();
                {
                    if ( IsSpin() || IsCustom() ) {
                        ImGui::BeginChild( XORSTR( "##ANTIAIM4" ), ImVec2( 0, 0 ), true );
                        {
                            if ( IsSpin() ) {
                                ImGui::Text( XORSTR( "Spin Options" ) );
                                ImGui::Separator();
                                ImGui::Columns( 2, NULL, true );
                                {
                                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                                    ImGui::Text( XORSTR( "Spin Factor" ) );
                                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                                }
                                ImGui::NextColumn();
                                {
                                    ImGui::PushItemWidth( -1 );
                                    ImGui::SliderInt( XORSTR( "##spinFactor" ), &Settings::spinFactor::value, 0, 100,
                                                      XORSTR( "%1.f" ) );
                                    ImGui::PopItemWidth();
                                }
                                ImGui::Columns( 1 );

                                ImGui::Separator();
                            }
                            if ( IsCustom() != 0 ) {
                                if ( IsCustom() >= 2 ) {
                                    ImGui::Text( XORSTR( "Custom Yaw" ) );
                                    ImGui::Separator();
                                    ImGui::PushItemWidth( -1 );
                                    ImGui::Checkbox( XORSTR( "Use LBY" ), &Settings::customYaw::lby );
                                    ImGui::SliderInt( XORSTR( "##customYaw" ), &Settings::customYaw::value,
                                                      !Settings::AntiAim::Moving::untrustedAngles ? -180 : -10000,
                                                      !Settings::AntiAim::Moving::untrustedAngles ? 180 : 10000,
                                                      XORSTR( "Amount: %1.f" ) );
                                    ImGui::PopItemWidth();
                                    ImGui::Separator();
                                }
                                if ( IsCustom() != 2 ) {
                                    ImGui::Text( XORSTR( "Custom Yaw 2" ) );
                                    ImGui::Separator();
                                    ImGui::PushItemWidth( -1 );
                                    ImGui::Checkbox( XORSTR( "Use LBY" ), &Settings::customYaw2::lby );
                                    ImGui::SliderInt( XORSTR( "##customYaw" ), &Settings::customYaw2::value,
                                                      !Settings::AntiAim::Moving::untrustedAngles ? -180 : -10000,
                                                      !Settings::AntiAim::Moving::untrustedAngles ? 180 : 10000,
                                                      XORSTR( "Amount: %1.f" ) );
                                    ImGui::PopItemWidth();
                                    ImGui::Separator();
                                }
                            }
                            ImGui::Separator();
                            ImGui::EndChild();
                        }
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
    }
}

bool Moving::IsSpin() {
    AntiAimType_Y aa = Settings::AntiAim::Moving::Yaw::type;
    AntiAimType_Y aaf = Settings::AntiAim::Moving::Yaw::typeFake;

    return aa == AntiAimType_Y::SPIN || aaf == AntiAimType_Y::SPIN ||
           aa == AntiAimType_Y::LBYSPIN || aaf == AntiAimType_Y::LBYSPIN;
}

int Moving::IsCustom() {
    AntiAimType_Y aa = Settings::AntiAim::Moving::Yaw::type;
    AntiAimType_Y aaf = Settings::AntiAim::Moving::Yaw::typeFake;

    if ( ( aa == AntiAimType_Y::CUSTOM || aaf == AntiAimType_Y::CUSTOM ) &&
         ( aa == AntiAimType_Y::CUSTOM2 || aaf == AntiAimType_Y::CUSTOM2 ) ) {
        return 3;
    } else if ( aa == AntiAimType_Y::CUSTOM || aaf == AntiAimType_Y::CUSTOM ) {
        return 2;
    } else if ( aa == AntiAimType_Y::CUSTOM2 || aaf == AntiAimType_Y::CUSTOM2 ) {
        return 1;
    } else {
        return 0;
    }
}
