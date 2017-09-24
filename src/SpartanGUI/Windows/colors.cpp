#include "colors.h"

bool Colors::showWindow = false;

void Colors::RenderWindow() {
    if ( !Colors::showWindow )
        return;

    struct ColorListVar {
        const char* name;

        union {
            ColorVar* colorVarPtr;
            HealthColorVar* healthColorVarPtr;
        };

        enum {
            COLORVAR_TYPE,
            HEALTHCOLORVAR_TYPE
        } type;

        ColorListVar( const char* name, ColorVar* ptr ) {
            this->name = name;
            this->colorVarPtr = ptr;
            this->type = COLORVAR_TYPE;
        }

        ColorListVar( const char* name, HealthColorVar* ptr ) {
            this->name = name;
            this->healthColorVarPtr = ptr;
            this->type = HEALTHCOLORVAR_TYPE;
        }
    };

    ColorListVar colors[] = {
            { XORSTR( "UI Main" ),                       &Settings::UI::mainColor },
            { XORSTR( "UI Body" ),                       &Settings::UI::bodyColor },
            { XORSTR( "UI Font" ),                       &Settings::UI::fontColor },
            { XORSTR( "UI Accent" ),                     &Settings::UI::accentColor },
            { XORSTR( "FOV Circle" ),                    &Settings::ESP::FOVCrosshair::color },
            { XORSTR( "Spread" ),                        &Settings::ESP::Spread::color },
            { XORSTR( "SpreadLimit" ),                   &Settings::ESP::Spread::spreadLimitColor },
            { XORSTR( "Hitmarker" ),                     &Settings::ESP::Hitmarker::color },
            { XORSTR( "ESP - Enemy" ),                   &Settings::ESP::enemyColor },
            { XORSTR( "ESP - Team" ),                    &Settings::ESP::allyColor },
            { XORSTR( "ESP - Enemy Visible" ),           &Settings::ESP::enemyVisibleColor },
            { XORSTR( "ESP - Team Visible" ),            &Settings::ESP::allyVisibleColor },
            { XORSTR( "ESP - CT" ),                      &Settings::ESP::ctColor },
            { XORSTR( "ESP - T" ),                       &Settings::ESP::tColor },
            { XORSTR( "ESP - CT Visible" ),              &Settings::ESP::ctVisibleColor },
            { XORSTR( "ESP - T Visible" ),               &Settings::ESP::tVisibleColor },
            { XORSTR( "ESP - LocalPlayer" ),             &Settings::ESP::localplayerColor },
            { XORSTR( "ESP - Bomb" ),                    &Settings::ESP::bombColor },
            { XORSTR( "ESP - Bomb Defusing" ),           &Settings::ESP::bombDefusingColor },
            { XORSTR( "ESP - Hostage" ),                 &Settings::ESP::hostageColor },
            { XORSTR( "ESP - Defuser" ),                 &Settings::ESP::defuserColor },
            { XORSTR( "ESP - Weapon" ),                  &Settings::ESP::weaponColor },
            { XORSTR( "ESP - Chicken" ),                 &Settings::ESP::chickenColor },
            { XORSTR( "ESP - Fish" ),                    &Settings::ESP::fishColor },
            { XORSTR( "ESP - Smoke" ),                   &Settings::ESP::smokeColor },
            { XORSTR( "ESP - Decoy" ),                   &Settings::ESP::decoyColor },
            { XORSTR( "ESP - Flashbang" ),               &Settings::ESP::flashbangColor },
            { XORSTR( "ESP - Grenade" ),                 &Settings::ESP::grenadeColor },
            { XORSTR( "ESP - Molotov" ),                 &Settings::ESP::molotovColor },
            { XORSTR( "ESP - Skeleton" ),                &Settings::ESP::Skeleton::color },
            { XORSTR( "Chams - Team" ),                  &Settings::ESP::Chams::allyColor },
            { XORSTR( "Chams - Team Visible" ),          &Settings::ESP::Chams::allyVisibleColor },
            { XORSTR( "Chams - Enemy" ),                 &Settings::ESP::Chams::enemyColor },
            { XORSTR( "Chams - Enemy Visible" ),         &Settings::ESP::Chams::enemyVisibleColor },
            { XORSTR( "Chams - LocalPlayer" ),           &Settings::ESP::Chams::localplayerColor },
            { XORSTR( "Chams - Arms" ),                  &Settings::ESP::Chams::Arms::color },
            { XORSTR( "Chams - Weapon" ),                &Settings::ESP::Chams::Weapon::color },
            { XORSTR( "Grenade Helper - Aim Line" ),     &Settings::GrenadeHelper::aimLine },
            { XORSTR( "Grenade Helper - Aim Dot" ),      &Settings::GrenadeHelper::aimDot },
            { XORSTR( "Grenade Helper - HE Info" ),      &Settings::GrenadeHelper::infoHE },
            { XORSTR( "Grenade Helper - Smoke Info" ),   &Settings::GrenadeHelper::infoSmoke },
            { XORSTR( "Grenade Helper - Molotov Info" ), &Settings::GrenadeHelper::infoMolotov },
            { XORSTR( "Grenade Helper - Flash Info" ),   &Settings::GrenadeHelper::infoFlash },
            { XORSTR( "Radar - Enemy" ),                 &Settings::Radar::enemyColor },
            { XORSTR( "Radar - Team" ),                  &Settings::Radar::allyColor },
            { XORSTR( "Radar - Enemy Visible" ),         &Settings::Radar::enemyVisibleColor },
            { XORSTR( "Radar - Team Visible" ),          &Settings::Radar::allyVisibleColor },
            { XORSTR( "Radar - CT" ),                    &Settings::Radar::ctColor },
            { XORSTR( "Radar - T" ),                     &Settings::Radar::tColor },
            { XORSTR( "Radar - CT Visible" ),            &Settings::Radar::ctVisibleColor },
            { XORSTR( "Radar - T Visible" ),             &Settings::Radar::tVisibleColor },
            { XORSTR( "Radar - Bomb" ),                  &Settings::Radar::bombColor },
            { XORSTR( "Radar - Bomb Defusing" ),         &Settings::Radar::bombDefusingColor },
            { XORSTR( "Glow - Team" ),                   &Settings::ESP::Glow::allyColor },
            { XORSTR( "Glow - Enemy" ),                  &Settings::ESP::Glow::enemyColor },
            { XORSTR( "Glow - Enemy Visible" ),          &Settings::ESP::Glow::enemyVisibleColor },
            { XORSTR( "Glow - LocalPlayer" ),            &Settings::ESP::Glow::localplayerColor },
            { XORSTR( "Glow - Weapon" ),                 &Settings::ESP::Glow::weaponColor },
            { XORSTR( "Glow - Grenade" ),                &Settings::ESP::Glow::grenadeColor },
            { XORSTR( "Glow - Defuser" ),                &Settings::ESP::Glow::defuserColor },
            { XORSTR( "Glow - Chicken" ),                &Settings::ESP::Glow::chickenColor },
            { XORSTR( "Sky" ),                           &Settings::NoSky::color },
            { XORSTR( "Walls" ),                         &Settings::ASUSWalls::color },
    };

    const char* colorNames[IM_ARRAYSIZE( colors )];
    for ( int i = 0; i < IM_ARRAYSIZE( colors ); i++ )
        colorNames[i] = colors[i].name;

    static int colorSelected = 0;

    ImGui::SetNextWindowSize( ImVec2( 540, 325 ), ImGuiCond_Always );
    if ( ImGui::Begin( XORSTR( "Colors" ), &Colors::showWindow,
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders |
                       ImGuiWindowFlags_NoResize ) ) {
        ImGui::Columns( 2, NULL, true );
        {
            ImGui::PushItemWidth( -1 );
            ImGui::ListBox( XORSTR( "##COLORSELECTION" ), &colorSelected, colorNames, IM_ARRAYSIZE( colorNames ), 12 );
            ImGui::PopItemWidth();
        }
        ImGui::NextColumn();
        {
            if ( colors[colorSelected].type == ColorListVar::HEALTHCOLORVAR_TYPE ) {
                UI::ColorPicker4( ( float* ) colors[colorSelected].healthColorVarPtr );
                ImGui::Checkbox( XORSTR( "Rainbow" ), &colors[colorSelected].healthColorVarPtr->rainbow );
                ImGui::SameLine();
                ImGui::Checkbox( XORSTR( "Health-Based" ), &colors[colorSelected].healthColorVarPtr->hp );
                ImGui::PushItemWidth( -1 );
                ImGui::SliderFloat( XORSTR( "##RAINBOWSPEED" ), &colors[colorSelected].healthColorVarPtr->rainbowSpeed,
                                    0.f, 1.f,
                                    XORSTR( "Rainbow Speed: %0.3f" ) );
                ImGui::PopItemWidth();
            } else {
                UI::ColorPicker4( ( float* ) colors[colorSelected].colorVarPtr );
                ImGui::Checkbox( XORSTR( "Rainbow" ), &colors[colorSelected].colorVarPtr->rainbow );
                ImGui::PushItemWidth( -1 );
                ImGui::SliderFloat( XORSTR( "##RAINBOWSPEED" ), &colors[colorSelected].colorVarPtr->rainbowSpeed, 0.f,
                                    1.f,
                                    XORSTR( "Rainbow Speed: %0.3f" ) );
                ImGui::PopItemWidth();
            }
        }
        ImGui::End();
    }
}
