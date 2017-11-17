#include "resolverinfo.h"

bool ResolverInfo::showWindow = false;

void ResolverInfo::RenderWindow() {
    if ( Settings::UI::Windows::ResolverInf::reload ) {
        ImGui::SetNextWindowPos(
                ImVec2( Settings::UI::Windows::ResolverInf::posX, Settings::UI::Windows::ResolverInf::posY ),
                ImGuiSetCond_Always );
        ImGui::SetNextWindowSize(
                ImVec2( Settings::UI::Windows::ResolverInf::sizeX, Settings::UI::Windows::ResolverInf::sizeY ),
                ImGuiSetCond_Always );
        Settings::UI::Windows::ResolverInf::reload = false;
        ResolverInfo::showWindow = Settings::UI::Windows::ResolverInf::open;
    } else {
        ImGui::SetNextWindowPos(
                ImVec2( Settings::UI::Windows::ResolverInf::posX, Settings::UI::Windows::ResolverInf::posY ),
                ImGuiSetCond_FirstUseEver );
        ImGui::SetNextWindowSize(
                ImVec2( Settings::UI::Windows::ResolverInf::sizeX, Settings::UI::Windows::ResolverInf::sizeY ),
                ImGuiSetCond_FirstUseEver );
    }
    if ( !ResolverInfo::showWindow ) {
        Settings::UI::Windows::ResolverInf::open = false;
        return;
    }


    if ( ImGui::Begin( XORSTR( "Resolver Info" ), NULL , ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders ) ) {
        Settings::UI::Windows::ResolverInf::open = true;
        ImVec2 temp = ImGui::GetWindowSize();
        Settings::UI::Windows::ResolverInf::sizeX = ( int ) temp.x;
        Settings::UI::Windows::ResolverInf::sizeY = ( int ) temp.y;
        temp = ImGui::GetWindowPos();
        Settings::UI::Windows::ResolverInf::posX = ( int ) temp.x;
        Settings::UI::Windows::ResolverInf::posY = ( int ) temp.y;
        if ( !engine->IsInGame() )
            ImGui::Text( XORSTR( "Not in-game\n" ) );
        else if ( !Settings::Resolver::enabled )
            ImGui::Text( XORSTR( "Resolver not enabled\n" ) );
        else {
            int playerId = Resolver::resolvingId;

            IEngineClient::player_info_t playerInfo;
            engine->GetPlayerInfo( playerId, &playerInfo );

            char* name = playerInfo.name;
            char* guid = playerInfo.guid;

            float lastHit = Math::ResNormalizeYaw( Resolver::lastHitAng[playerId] );
            float angForce = Math::ResNormalizeYaw( Resolver::angForce[playerId] );
            const char* angForceTxt = Resolver::angForceTxt[playerId];
            int shotsMissed = Resolver::shotsMiss[playerId];
            float lby = Math::ResNormalizeYaw( Resolver::lby[playerId] );

            ImGui::Text( XORSTR( "%s [%s]\n" ), name, guid );
            ImGui::Text( XORSTR( "Trying Angle : %s [%.1f]\n" ), angForceTxt, angForce );
            ImGui::Text( XORSTR( "Last Hit : %.1f\n" ), lastHit );
            ImGui::Text( XORSTR( "Shots missed : %d\n" ), shotsMissed );
            ImGui::Text( XORSTR( "LBY : %.1f\n" ), lby );
        }

        ImGui::End();
    }
}