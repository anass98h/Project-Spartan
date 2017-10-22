#include "resolverinfo.h"

bool ResolverInfo::showWindow = false;

void ResolverInfo::RenderWindow() {
    if ( !ResolverInfo::showWindow )
        return;

    ImGui::SetNextWindowSize( ImVec2( 200, 200 ), ImGuiCond_Always );

    if ( ImGui::Begin( XORSTR( "Resolver Info" ) ) ) {
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
    
            float lastHit = Resolver::lastHitAng[playerId];
            float angForce = Resolver::angForce[playerId];
            const char* angForceTxt = Resolver::angForceTxt[playerId];
            int shotsMissed = Resolver::shotsMiss[playerId];
            float lby = Resolver::lby[playerId];
    
            ImGui::Text( "%s [%s]\n", name, guid );
            ImGui::Text( "Last hit - %i\n", lastHit );
            ImGui::Text( "Trying Angle - %s [%i]\n", angForceTxt, angForce );
            ImGui::Text( "Last Hit - %i\n", lastHit );
            ImGui::Text( "Shots missed - %i\n", shotsMissed );
            ImGui::Text( "LBY - %i\n", lby );
        }
        
        ImGui::End();
    }
}