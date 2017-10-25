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
    
            float lastHit = Math::ResNormalizeYaw( Resolver::lastHitAng[playerId] );
            float angForce = Math::ResNormalizeYaw( Resolver::angForce[playerId] );
            const char* angForceTxt = Resolver::angForceTxt[playerId];
            int shotsMissed = Resolver::shotsMiss[playerId];
            float lby = Math::ResNormalizeYaw( Resolver::lby[playerId] );
    
            ImGui::Text( XORSTR( "%s [%s]\n" ), name, guid );
            ImGui::Text( XORSTR( "Last hit - %.1f\n" ), lastHit );
            ImGui::Text( XORSTR( "Trying Angle - %s [%.1f]\n" ), angForceTxt, angForce );
            ImGui::Text( XORSTR( "Last Hit - %.1f\n" ), lastHit );
            ImGui::Text( XORSTR( "Shots missed - %d\n" ), shotsMissed );
            ImGui::Text( XORSTR( "LBY - %.1f\n" ), lby );
        }
        
        ImGui::End();
    }
}