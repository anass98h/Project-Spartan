#include "misctab.h"

static char nickname[127] = "";

void Misc::RenderTab() {
    const char* strafeTypes[] = { "Forwards", "Backwards", "Left", "Right", "Rage" };
    const char* animationTypes[] = { "Static", "Current Time", "Marquee", "Words", "Letters" };
    const char* spammerTypes[] = { "None", "Normal", "Positions" };
    const char* teams[] = { "Allies", "Enemies", "Both" };
    const char* presetTypes[] = { "Project-Spartan", "cantvac.me", "tuxcheats.com", "realnigga.club",
                                  "void MarcIsAWeeb", "Custom" };
    const char* lagTypes[] = { "Off", "AimTux", "Normal", "Step", "Reactive", "Nuclear", "Lunico" };
    const char* grenadeTypes[] = { "Flashbang", "Smoke Grenade", "Molotov", "HE Grenade" };
    const char* throwTypes[] = { "Standing", "Running", "Jumping", "Walk" };
    const char* tpModes[] = { "Fake", "Real", "Lower Body Yaw", "Ghost" };

    ImGui::Columns( 2, NULL, true );
    {
        ImGui::BeginChild( XORSTR( "Child1" ), ImVec2( 0, 0 ), true );
        {
            ImGui::Text( XORSTR( "Movement" ) );
            ImGui::Separator();

            ImGui::Checkbox( XORSTR( "Bunny Hop" ), &Settings::BHop::enabled );

            ImGui::Columns( 2, NULL, true );
            {
                ImGui::Checkbox( XORSTR( "Edge Jump" ), &Settings::EdgeJump::enabled );
            }
            ImGui::NextColumn();
            {
                UI::KeyBindButton( &Settings::EdgeJump::key );
            }
            ImGui::Columns( 1 );
            ImGui::Separator();
            ImGui::Columns( 2, NULL, true );
            {
                ImGui::Checkbox( XORSTR( "Auto Strafe" ), &Settings::AutoStrafe::enabled );
            }
            ImGui::NextColumn();
            {
                ImGui::PushItemWidth( -1 );
                ImGui::Combo( XORSTR( "##STRAFETYPE" ), ( int* ) &Settings::AutoStrafe::type, strafeTypes,
                              IM_ARRAYSIZE( strafeTypes ) );
                ImGui::PopItemWidth();
            }

            if ( Settings::AutoStrafe::type == AutostrafeType::AS_RAGE ) {
                ImGui::Checkbox( XORSTR( "Silent" ), &Settings::AutoStrafe::silent );
            }

            ImGui::Columns( 1 );
            ImGui::Separator();
            ImGui::Text( XORSTR( "Spammer" ) );
            ImGui::Separator();

            ImGui::Columns( 3, NULL, true );
            {
                ImGui::Checkbox( XORSTR( "Kill Messages" ), &Settings::Spammer::KillSpammer::enabled );
            }
            ImGui::NextColumn();
            {
                ImGui::Checkbox( XORSTR( "Team Chat###SAY_TEAM1" ), &Settings::Spammer::KillSpammer::sayTeam );
            }
            ImGui::NextColumn();
            {
                if ( ImGui::Button( XORSTR( "Options###KILL" ) ) )
                    ImGui::OpenPopup( XORSTR( "options_kill" ) );

                ImGui::SetNextWindowSize( ImVec2( 565, 268 ), ImGuiSetCond_Always );
                if ( ImGui::BeginPopup( XORSTR( "options_kill" ) ) ) {
                    static int killSpammerMessageCurrent = -1;
                    static char killSpammerMessageBuf[126];

                    ImGui::PushItemWidth( 445 );
                    ImGui::InputText( XORSTR( "###SPAMMERMESSAGE" ), killSpammerMessageBuf,
                                      IM_ARRAYSIZE( killSpammerMessageBuf ) );
                    ImGui::PopItemWidth();
                    ImGui::SameLine();

                    if ( ImGui::Button( XORSTR( "Add" ) ) ) {
                        if ( strlen( killSpammerMessageBuf ) > 0 )
                            Settings::Spammer::KillSpammer::messages.push_back( std::string( killSpammerMessageBuf ) );

                        strcpy( killSpammerMessageBuf, "" );
                    }
                    ImGui::SameLine();

                    if ( ImGui::Button( XORSTR( "Remove" ) ) )
                        if ( killSpammerMessageCurrent > -1 &&
                             ( int ) Settings::Spammer::KillSpammer::messages.size() > killSpammerMessageCurrent )
                            Settings::Spammer::KillSpammer::messages.erase(
                                    Settings::Spammer::KillSpammer::messages.begin() + killSpammerMessageCurrent );

                    ImGui::PushItemWidth( 550 );
                    ImGui::ListBox( "", &killSpammerMessageCurrent, Settings::Spammer::KillSpammer::messages, 10 );
                    ImGui::PopItemWidth();

                    ImGui::EndPopup();
                }
            }

            ImGui::Columns( 1 );
            ImGui::Checkbox( XORSTR( "Radio Commands" ), &Settings::Spammer::RadioSpammer::enabled );

            ImGui::Columns( 3, NULL, true );
            {
                ImGui::Combo( XORSTR( "###SPAMMERYPE" ), ( int* ) &Settings::Spammer::type, spammerTypes,
                              IM_ARRAYSIZE( spammerTypes ) );
            }
            ImGui::NextColumn();
            {
                ImGui::Checkbox( XORSTR( "Team Chat###SAY_TEAM2" ), &Settings::Spammer::say_team );
            }
            ImGui::NextColumn();
            {
                if ( Settings::Spammer::type != SpammerType::SPAMMER_NONE &&
                     ImGui::Button( XORSTR( "Options###SPAMMER" ) ) )
                    ImGui::OpenPopup( XORSTR( "options_spammer" ) );

                if ( Settings::Spammer::type == SpammerType::SPAMMER_NORMAL )
                    ImGui::SetNextWindowSize( ImVec2( 565, 268 ), ImGuiSetCond_Always );
                else if ( Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS )
                    ImGui::SetNextWindowSize( ImVec2( 200, 240 ), ImGuiSetCond_Always );

                if ( Settings::Spammer::type != SpammerType::SPAMMER_NONE &&
                     ImGui::BeginPopup( XORSTR( "options_spammer" ) ) ) {
                    if ( Settings::Spammer::type == SpammerType::SPAMMER_NORMAL ) {
                        static int spammerMessageCurrent = -1;
                        static char spammerMessageBuf[126];

                        ImGui::PushItemWidth( 445 );
                        ImGui::InputText( XORSTR( "###SPAMMERMESSAGE" ), spammerMessageBuf,
                                          IM_ARRAYSIZE( spammerMessageBuf ) );
                        ImGui::PopItemWidth();
                        ImGui::SameLine();

                        if ( ImGui::Button( XORSTR( "Add" ) ) ) {
                            if ( strlen( spammerMessageBuf ) > 0 )
                                Settings::Spammer::NormalSpammer::messages.push_back(
                                        std::string( spammerMessageBuf ) );

                            strcpy( spammerMessageBuf, "" );
                        }
                        ImGui::SameLine();

                        if ( ImGui::Button( XORSTR( "Remove" ) ) )
                            if ( spammerMessageCurrent > -1 &&
                                 ( int ) Settings::Spammer::NormalSpammer::messages.size() > spammerMessageCurrent )
                                Settings::Spammer::NormalSpammer::messages.erase(
                                        Settings::Spammer::NormalSpammer::messages.begin() + spammerMessageCurrent );

                        ImGui::PushItemWidth( 550 );
                        ImGui::ListBox( "", &spammerMessageCurrent, Settings::Spammer::NormalSpammer::messages, 10 );
                        ImGui::PopItemWidth();
                    } else if ( Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS ) {
                        ImGui::PushItemWidth( 185 );
                        ImGui::Combo( XORSTR( "###POSITIONSTEAM" ), &Settings::Spammer::PositionSpammer::team, teams,
                                      IM_ARRAYSIZE( teams ) );
                        ImGui::PopItemWidth();
                        ImGui::Separator();
                        ImGui::Checkbox( XORSTR( "Show Name" ), &Settings::Spammer::PositionSpammer::showName );
                        ImGui::Checkbox( XORSTR( "Show Weapon" ), &Settings::Spammer::PositionSpammer::showWeapon );
                        ImGui::Checkbox( XORSTR( "Show Rank" ), &Settings::Spammer::PositionSpammer::showRank );
                        ImGui::Checkbox( XORSTR( "Show Wins" ), &Settings::Spammer::PositionSpammer::showWins );
                        ImGui::Checkbox( XORSTR( "Show Health" ), &Settings::Spammer::PositionSpammer::showHealth );
                        ImGui::Checkbox( XORSTR( "Show Money" ), &Settings::Spammer::PositionSpammer::showMoney );
                        ImGui::Checkbox( XORSTR( "Show Last Place" ),
                                         &Settings::Spammer::PositionSpammer::showLastplace );
                    }

                    ImGui::EndPopup();
                }

                ImGui::Columns( 1 );
                ImGui::Separator();
                ImGui::Text( XORSTR( "FOV" ) );
                ImGui::Separator();
                ImGui::Columns( 2, NULL, true );
                {
                    ImGui::Checkbox( XORSTR( "FOV" ), &Settings::FOVChanger::enabled );
                    ImGui::Checkbox( XORSTR( "Viewmodel FOV" ), &Settings::FOVChanger::viewmodelEnabled );
                    ImGui::Checkbox( XORSTR( "Ignore Scope" ), &Settings::FOVChanger::ignoreScope );
                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth( -1 );
                    ImGui::SliderFloat( XORSTR( "##FOVAMOUNT" ), &Settings::FOVChanger::value, 0, 180 );
                    ImGui::SliderFloat( XORSTR( "##MODELFOVAMOUNT" ), &Settings::FOVChanger::viewmodelValue, 0, 360 );
                    ImGui::PopItemWidth();
                }
                ImGui::Columns( 1 );
                ImGui::Separator();

                ImGui::Checkbox( XORSTR( "Third Person" ), &Settings::ThirdPerson::enabled );
                {
                    ImGui::Separator();

                    ImGui::Columns( 2, NULL, true );
                    {
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Mode" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Camera Offset" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Key" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                        ImGui::Text( XORSTR( "Transparency (Scoped)" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }
                    ImGui::NextColumn();
                    {
                        ImGui::PushItemWidth( -3 );
                        ImGui::Combo( XORSTR( "##TPMODE" ), ( int* ) &Settings::ThirdPerson::mode,
                                      tpModes, ( int ) ThirdPersonMode::NUMBER_OF_TYPES );
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##TPCAMOFFSET" ), &Settings::ThirdPerson::distance, 0.0f, 500.0f );
                        ImGui::PopItemWidth();
                        UI::KeyBindButton( &Settings::ThirdPerson::key );
                        ImGui::PushItemWidth( -1 );
                        ImGui::SliderFloat( XORSTR( "##TRANSPARENCY" ), &Settings::ThirdPerson::transparency, 0.1f,
                                            0.6f, XORSTR( "%0.1f" ) );
                        ImGui::PopItemWidth();
                    }
                    ImGui::Columns( 1 );
                }
                ImGui::Separator();

                ImGui::Text( XORSTR( "Grenade Helper" ) );
                ImGui::Separator();
                ImGui::Columns( 2, NULL, true );
                {
                    ImGui::Checkbox( XORSTR( "Enabled ###ghenabled" ), &Settings::GrenadeHelper::enabled );
                }
                ImGui::NextColumn();
                {
                    ImGui::Checkbox( XORSTR( "Only matching ###match" ), &Settings::GrenadeHelper::onlyMatchingInfos );
                }
                ImGui::Columns( 1 );
                ImGui::Columns( 2, NULL, true );
                {
                    if ( ImGui::Button( XORSTR( "Aimassist" ), ImVec2( -1, 0 ) ) )
                        ImGui::OpenPopup( XORSTR( "optionAimAssist" ) );
                    ImGui::SetNextWindowSize( ImVec2( 200, 120 ), ImGuiSetCond_Always );
                    if ( ImGui::BeginPopup( XORSTR( "optionAimAssist" ) ) ) {
                        ImGui::PushItemWidth( -1 );
                        ImGui::Checkbox( XORSTR( "Enabled" ), &Settings::GrenadeHelper::aimAssist );
                        ImGui::SliderFloat( XORSTR( "###aimstep" ), &Settings::GrenadeHelper::aimStep, 0, 10,
                                            "Speed: %0.3f" );
                        ImGui::SliderFloat( XORSTR( "###aimfov" ), &Settings::GrenadeHelper::aimFov, 0, 180,
                                            "Fov: %0.2f" );
                        ImGui::SliderFloat( XORSTR( "###aimdistance" ), &Settings::GrenadeHelper::aimDistance, 0, 100,
                                            "Distance: %0.2f" );
                        ImGui::PopItemWidth();
                        ImGui::EndPopup();
                    }
                }
                ImGui::NextColumn();
                {
                    if ( ImGui::Button( XORSTR( "Add Info" ), ImVec2( -1, 0 ) ) )
                        ImGui::OpenPopup( XORSTR( "addinfo_throw" ) );

                    ImGui::SetNextWindowSize( ImVec2( 565, 268 ), ImGuiSetCond_Always );
                    if ( ImGui::BeginPopup( XORSTR( "addinfo_throw" ) ) ) {
                        static int throwMessageCurrent = -1;
                        static char inputName[40];
                        static int tType = ( int ) ThrowType::NORMAL;
                        static int gType = ( int ) GrenadeType::SMOKE;

                        if ( engine->IsInGame() ) {
                            C_BasePlayer* localPlayer = ( C_BasePlayer* ) entityList->GetClientEntity(
                                    engine->GetLocalPlayer() );
                            if ( localPlayer ) {
                                C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
                                        localPlayer->GetActiveWeapon() );
                                if ( activeWeapon &&
                                     activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE )
                                    gType = ( int ) GetGrenadeType( activeWeapon );
                            }
                        }
                        ImGui::Columns( 1 );
                        ImGui::PushItemWidth( 500 );
                        ImGui::InputText( "", inputName, sizeof( inputName ) );
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                        if ( ImGui::Button( XORSTR( "Add" ) ) && engine->IsInGame() &&
                             Settings::GrenadeHelper::actMapName.length() > 0 ) {
                            C_BasePlayer* localPlayer = ( C_BasePlayer* ) entityList->GetClientEntity(
                                    engine->GetLocalPlayer() );
                            if ( strlen( inputName ) > 0 ) {
                                GrenadeInfo n = GrenadeInfo( ( GrenadeType ) gType, localPlayer->GetEyePosition(),
                                                             *localPlayer->GetVAngles(), ( ThrowType ) tType,
                                                             inputName );
                                Settings::GrenadeHelper::grenadeInfos.push_back( n );
                                std::ostringstream path;
                                path << GetGhConfigDirectory() << Settings::GrenadeHelper::actMapName;
                                if ( !DoesFileExist( path.str().c_str() ) )
                                    mkdir( path.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
                                path << XORSTR( "/config.json" );
                                Settings::SaveGrenadeInfo( path.str() );
                            }
                            strcpy( inputName, "" );
                        }
                        ImGui::Columns( 2 );
                        ImGui::Combo( XORSTR( "###Throwtype" ), &tType, throwTypes, IM_ARRAYSIZE( throwTypes ) );
                        ImGui::NextColumn();
                        ImGui::Combo( XORSTR( "###Grenadetype" ), &gType, grenadeTypes, IM_ARRAYSIZE( grenadeTypes ) );
                        ImGui::Columns( 1 );
                        ImGui::Separator();
                        ImGui::PushItemWidth( 550 );
                        auto lambda = []( void* data, int idx, const char** out_text ) {
                            *out_text = Settings::GrenadeHelper::grenadeInfos.at( idx ).name.c_str();
                            return *out_text != NULL;
                        };
                        ImGui::ListBox( "", &throwMessageCurrent, lambda, NULL,
                                        Settings::GrenadeHelper::grenadeInfos.size(), 7 );
                        ImGui::PopItemWidth();
                        ImGui::Columns( 1 );
                        if ( ImGui::Button( XORSTR( "Remove" ), ImVec2( ImGui::GetWindowWidth(), 30 ) ) )
                            if ( throwMessageCurrent > -1 &&
                                 ( int ) Settings::GrenadeHelper::grenadeInfos.size() > throwMessageCurrent ) {
                                Settings::GrenadeHelper::grenadeInfos.erase(
                                        Settings::GrenadeHelper::grenadeInfos.begin() + throwMessageCurrent );
                                std::ostringstream path;
                                path << GetGhConfigDirectory() << Settings::GrenadeHelper::actMapName;
                                if ( !DoesFileExist( path.str().c_str() ) )
                                    mkdir( path.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
                                path << XORSTR( "/config.json" );
                                Settings::SaveGrenadeInfo( path.str() );
                            }
                        ImGui::EndPopup();
                    }
                }
                ImGui::Columns( 1 );
                ImGui::Separator();
                ImGui::EndChild();
            }
        }
        ImGui::NextColumn();
        {
            ImGui::BeginChild( XORSTR( "Child2" ), ImVec2( 0, 0 ), true );
            {
                ImGui::Text( XORSTR( "Clantag" ) );
                ImGui::Separator();
                ImGui::Checkbox( XORSTR( "Enabled" ), &Settings::ClanTagChanger::enabled );
                ImGui::Separator();
                ImGui::Columns( 1 );

                ImGui::Separator();
                ImGui::Columns( 2, NULL, true );
                {
                    ImGui::PushItemWidth( -1 );
                    if ( ImGui::Combo( XORSTR( "##PRESETTYPE" ), ( int* ) &Settings::ClanTagChanger::preset,
                                       presetTypes,
                                       IM_ARRAYSIZE( presetTypes ) ) )
                        ClanTagChanger::UpdateClanTagCallback();
                    ImGui::PopItemWidth();

                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    ImGui::Text( XORSTR( "Animation Speed" ) );
                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );

                    if ( Settings::ClanTagChanger::preset == valueType::CUSTOM ) {
                        ImGui::Text( XORSTR( "Custom Clan Tag" ) );
                        ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    }

                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth( -1 );
                    if ( ImGui::Combo( XORSTR( "##ANIMATIONTYPE" ), ( int* ) &Settings::ClanTagChanger::type,
                                       animationTypes, IM_ARRAYSIZE( animationTypes ) ) )
                        ClanTagChanger::UpdateClanTagCallback();
                    if ( ImGui::SliderInt( XORSTR( "##ANIMATIONSPEED" ), &Settings::ClanTagChanger::animationSpeed, 0,
                                           2000 ) )
                        ClanTagChanger::UpdateClanTagCallback();

                    if ( Settings::ClanTagChanger::preset == valueType::CUSTOM ) {
                        if ( ImGui::InputText( XORSTR( "##CLANTAG" ), Settings::ClanTagChanger::value, 30 ) ) {
                            if(Settings::ClanTagChanger::value[1] != 0 )
                            ClanTagChanger::UpdateClanTagCallback();

                        }
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::Columns( 1 );
                ImGui::Separator();
                ImGui::Text( XORSTR( "Nickname" ) );
                ImGui::Separator();

                ImGui::InputText( XORSTR( "##NICKNAME" ), nickname, 127 );

                ImGui::SameLine();
                if ( ImGui::Button( XORSTR( "Set Nickname" ), ImVec2( -1, 0 ) ) )
                    NameChanger::SetName( std::string( nickname ).c_str() );

                if ( ImGui::Button( XORSTR( "Glitch Name" ) ) )
                    NameChanger::SetName( "\n\xAD\xAD\xAD" );
                ImGui::SameLine();
                if ( ImGui::Button( XORSTR( "No Name" ) ) )
                    ImGui::OpenPopup( XORSTR( "optionNoName" ) );
                ImGui::SetNextWindowSize( ImVec2( 150, 100 ), ImGuiCond_Always );
                if ( ImGui::BeginPopup( XORSTR ( "optionNoName" ) ) ) {
                    ImGui::PushItemWidth( -1 );
                    for ( auto& it : NameChanger::nntypes ) {
                        if ( ImGui::Button( it.second, ImVec2( -1, 0 ) ) )
                            NameChanger::InitNoName( NameChanger::NC_Type::NC_NORMAL, it.first );

                    }
                    ImGui::PopItemWidth();

                    ImGui::EndPopup();
                }

                ImGui::SameLine();
                if ( ImGui::Button( XORSTR( "Infinite Name Spam" ) ) )
                    NameChanger::InitColorChange( NameChanger::NC_Type::NC_RAINBOW );

                ImGui::SameLine();
                if ( ImGui::Button( XORSTR( "Colorize Name" ), ImVec2( -1, 0 ) ) )
                    ImGui::OpenPopup( XORSTR( "optionColorizeName" ) );
                ImGui::SetNextWindowSize( ImVec2( 150, 300 ), ImGuiSetCond_Always );
                if ( ImGui::BeginPopup( XORSTR( "optionColorizeName" ) ) ) {
                    ImGui::PushItemWidth( -1 );
                    for ( auto& it : NameChanger::colors ) {
                        if ( ImGui::Button( it.second, ImVec2( -1, 0 ) ) )
                            NameChanger::InitColorChange( NameChanger::NC_Type::NC_SOLID, it.first );
                    }
                    ImGui::PopItemWidth();

                    ImGui::EndPopup();
                }
                ImGui::Columns( 2, NULL, true );
                {
                    if ( ImGui::Checkbox( XORSTR( "Name Stealer" ), &Settings::NameStealer::enabled ) )
                        NameStealer::entityId = -1;
                }
                ImGui::NextColumn();
                {
                    ImGui::Combo( "", &Settings::NameStealer::team, teams, IM_ARRAYSIZE( teams ) );
                }

                ImGui::Columns( 1 );
                ImGui::Separator();

                ImGui::Text( XORSTR( "Fake Lag" ) );
                ImGui::Separator();
                ImGui::Columns( 2, NULL, true );
                {
                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    ImGui::Text( XORSTR( "Type" ) );
                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                    if(Settings::FakeLag::type != FakeLagType::LUNICO)
                    ImGui::Text( XORSTR( "Choke Amount" ) );
                    ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
                }
                ImGui::NextColumn();
                {
                    ImGui::PushItemWidth( -3 );
                    ImGui::Combo( XORSTR( "##FAKELAGTYPE" ), ( int* ) &Settings::FakeLag::type,
                                  lagTypes, IM_ARRAYSIZE( lagTypes ) );
                    ImGui::PopItemWidth();
                    ImGui::PushItemWidth( -1 );
                    if(Settings::FakeLag::type != FakeLagType::LUNICO) {
                        ImGui::SliderInt(XORSTR("##FAKELAGAMOUNT"), &Settings::FakeLag::value, 0, 16,
                                         XORSTR("Amount: %0.f"));
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::EndColumns();
                ImGui::Separator();

                ImGui::Text( XORSTR( "Other" ) );
                ImGui::Separator();
                ImGui::Columns( 2, NULL, true );
                {
                    ImGui::Checkbox( XORSTR( "Auto Accept" ), &Settings::AutoAccept::enabled );
                    SetTooltip( XORSTR( "WARNING: This may crash your game." ) );
                    ImGui::Checkbox( XORSTR( "AirStuck" ), &Settings::Airstuck::enabled );
                    ImGui::Checkbox( XORSTR( "Autoblock" ), &Settings::Autoblock::enabled );
                    ImGui::Checkbox( XORSTR( "Jump Throw" ), &Settings::JumpThrow::enabled );
                    ImGui::Checkbox( XORSTR( "Auto Defuse" ), &Settings::AutoDefuse::enabled );
                    ImGui::Checkbox( XORSTR( "Sniper Crosshair" ), &Settings::SniperCrosshair::enabled );
                    ImGui::Checkbox( XORSTR( "Disable post-processing" ), &Settings::DisablePostProcessing::enabled );
                    ImGui::Checkbox( XORSTR( "In-Game Watermark" ), &Settings::UI::Watermark::displayIngame );
                }
                ImGui::NextColumn();
                {
                    ImGui::Checkbox( XORSTR( "Show Ranks" ), &Settings::ShowRanks::enabled );
                    UI::KeyBindButton( &Settings::Airstuck::key );
                    UI::KeyBindButton( &Settings::Autoblock::key );
                    UI::KeyBindButton( &Settings::JumpThrow::key );
                    //ImGui::Checkbox(XORSTR("Smart Aim"), &Settings::SmartAim::enabled);
                    ImGui::Checkbox( XORSTR( "Silent Defuse" ), &Settings::AutoDefuse::silent );
                    ImGui::Checkbox( XORSTR( "Event Logger" ), &Settings::EventLogger::enabled );
                    ImGui::Checkbox( XORSTR( "Screenshot Cleaner" ), &Settings::ScreenshotCleaner::enabled );
                }
                ImGui::Columns( 1 );
                ImGui::Separator();

                ImGui::PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 210, 85 ) );
                if ( ImGui::BeginPopupModal( XORSTR( "Error###UNTRUSTED_FEATURE" ) ) ) {
                    ImGui::Text( XORSTR( "You cannot use this feature on a VALVE server." ) );

                    ImGui::Checkbox( XORSTR( "This is not a VALVE server" ), &ValveDSCheck::forceUT );

                    if ( ImGui::Button( XORSTR( "OK" ) ) )
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar();

                ImGui::EndChild();
            }
        }
    }
}
