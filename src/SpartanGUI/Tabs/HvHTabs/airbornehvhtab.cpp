#include "airbornehvhtab.h"

void Airborne::RenderTab() {
    ImGui::Columns( 2, NULL, true );
    {
        ImGui::BeginChild( XORSTR( "##ANTIAIM1" ), ImVec2( 0, 0 ), true );
        {
            ImGui::Text( XORSTR( "Anti-Aim" ) );
            ImGui::BeginChild( XORSTR( "##ANTIAIM3" ), ImVec2( 0, 0 ), true );
            {
                ImGui::Checkbox( XORSTR( "Yaw" ), &Settings::AntiAim::Airborne::Yaw::enabled );
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
                            ImGui::Combo( XORSTR( "##YFAKE" ), ( int* ) &Settings::AntiAim::Airborne::Yaw::typeFake,
                                          HvH::yTypes, (int)AntiAimType_Y::NUMBER_OF_TYPES );
                            ImGui::PopItemWidth();
                            ImGui::PushItemWidth( -1 );
                            ImGui::SliderFloat( XORSTR( "##YFAKECUSTOM" ),
                                                &Settings::AntiAim::Airborne::Yaw::typeFakeAdd,
                                                Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
                                                Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
                                                XORSTR( Settings::AntiAim::Airborne::Yaw::typeFakeAdd > 0 ? "+%.1f"
                                                                                                          : "%.1f" ) );
                            ImGui::PopItemWidth();
                        }
                        ImGui::Separator();
                        {
                            ImGui::PushItemWidth( -3 );
                            ImGui::Combo( XORSTR( "##YREAL" ), ( int* ) &Settings::AntiAim::Airborne::Yaw::type,
                                          HvH::yTypes, (int)AntiAimType_Y::NUMBER_OF_TYPES );
                            ImGui::PopItemWidth();
                            ImGui::PushItemWidth( -1 );
                            ImGui::SliderFloat( XORSTR( "##YCUSTOM" ), &Settings::AntiAim::Airborne::Yaw::typeAdd,
                                                Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
                                                Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
                                                XORSTR( Settings::AntiAim::Airborne::Yaw::typeAdd > 0 ? "+%.1f"
                                                                                                      : "%.1f" ) );
                            ImGui::PopItemWidth();
                        }
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Pitch" ), &Settings::AntiAim::Airborne::Pitch::enabled );
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
                        ImGui::Combo( XORSTR( "##XREAL" ), ( int* ) &Settings::AntiAim::Airborne::Pitch::type,
                                      HvH::xTypes, (int)AntiAimType_X::NUMBER_OF_TYPES );
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##XCUSTOM" ), &Settings::AntiAim::Airborne::Pitch::custom,
                                            Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
                                            Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
                                            XORSTR( Settings::AntiAim::Airborne::Pitch::custom > 0 ? "+%.1f"
                                                                                                   : "%.1f" ) );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Roll" ), &Settings::AntiAim::Airborne::Roll::enabled );
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
                        ImGui::Combo( XORSTR( "##ZREAL" ), ( int* ) &Settings::AntiAim::Airborne::Roll::type,
                                      HvH::zTypes, (int)AntiAimType_Z::NUMBER_OF_TYPES );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Switch Anti-Aim" ), &Settings::AntiAim::Airborne::SwitchAA::enabled );
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
                        UI::KeyBindButton( &Settings::AntiAim::Airborne::SwitchAA::key );
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "LBY Breaker" ), &Settings::AntiAim::Airborne::LBY::enabled );
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
                        ImGui::Combo( XORSTR( "##LBYREAL" ), ( int* ) &Settings::AntiAim::Airborne::LBY::type,
                                      HvH::lbyTypes, (int)AntiAimType_LBY::NUMBER_OF_TYPES );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Head Edge" ), &Settings::AntiAim::Airborne::HeadEdge::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Edge Distance" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Edge Real Add" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Edge Fake Add" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##EDGEDISTANCE" ),
                                            &Settings::AntiAim::Airborne::HeadEdge::distance, 20, 30
                        );
                        ImGui::SliderFloat( XORSTR( "##EDGEREALADD" ),
                                            &Settings::AntiAim::Airborne::HeadEdge::realAdd,
                                            Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
                                            Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
                                            XORSTR( Settings::AntiAim::Airborne::HeadEdge::realAdd > 0 ? "+%.1f"
                                                                                                       : "%.1f" )
                        );
                        ImGui::SliderFloat( XORSTR( "##EDGEFAKEADD" ),
                                            &Settings::AntiAim::Airborne::HeadEdge::fakeAdd,
                                            Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
                                            Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
                                            XORSTR( Settings::AntiAim::Airborne::HeadEdge::fakeAdd > 0 ? "+%.1f"
                                                                                                       : "%.1f" )
                        );
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
                ImGui::Checkbox( XORSTR( "Anti Resolver" ), &Settings::AntiAim::Airborne::antiResolver );
                ImGui::Separator();
                ImGui::Checkbox( XORSTR( "Dynamic Anti-Aim" ), &Settings::AntiAim::Airborne::dynamicAA );
                ImGui::Separator();
                ImGui::Checkbox( XORSTR( "Untrusted angles" ), &Settings::AntiAim::Airborne::untrustedAngles );
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
                                                      !Settings::AntiAim::Airborne::untrustedAngles ? -180 : -10000,
                                                      !Settings::AntiAim::Airborne::untrustedAngles ? 180 : 10000,
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
                                                      !Settings::AntiAim::Airborne::untrustedAngles ? -180 : -10000,
                                                      !Settings::AntiAim::Airborne::untrustedAngles ? 180 : 10000,
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

bool Airborne::IsSpin() {
    AntiAimType_Y aa = Settings::AntiAim::Airborne::Yaw::type;
    AntiAimType_Y aaf = Settings::AntiAim::Airborne::Yaw::typeFake;

    return aa == AntiAimType_Y::SPIN || aaf == AntiAimType_Y::SPIN ||
           aa == AntiAimType_Y::LBYSPIN || aaf == AntiAimType_Y::LBYSPIN;
}

int Airborne::IsCustom() {
    AntiAimType_Y aa = Settings::AntiAim::Airborne::Yaw::type;
    AntiAimType_Y aaf = Settings::AntiAim::Airborne::Yaw::typeFake;

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
