#include "resolverinfo.h"

bool ResolverInfo::showWindow = false;

void ResolverInfo::RenderWindow() {
    if ( !ResolverInfo::showWindow )
        return;

    ImGui::SetNextWindowSize( ImVec2( 200, 200 ), ImGuiCond_Always );

    if ( ImGui::Begin( XORSTR( "Resolver Info" ) ) ) {
        if ( !engine->IsInGame() ) {
            ImGui::Text( XORSTR( "" ) );
        } else if ( !Settings::Resolver::enabled ) {
            ImGui::Text( XORSTR( "" ) );
        } else {
            int playerId = Resolver::resolvingId;
    
            IEngineClient::player_info_t playerInfo;
            engine->GetPlayerInfo( playerId, &playerInfo );
    
            char* name = playerInfo.name;
            char* guid = playerInfo.guid;

            float lastHit = Math::RoundFloat( Resolver::lastHitAng[playerId] );
            float angForce = Math::RoundFloat( Resolver::angForce[playerId] );
            const char* angForceTxt = Resolver::angForceTxt[playerId];
            int shotsMissed = Resolver::shotsMissedSave[playerId];
            float lby = Math::RoundFloat( Resolver::lby[playerId] );

            ImGui::Text( XORSTR( "%s [%s]" ), name, guid );
            ImGui::Text( XORSTR( "Trying Angle - %s [%f]" ), angForceTxt, angForce );
            ImGui::Text( XORSTR( "Last Hit - %f" ), lastHit );
            ImGui::Text( XORSTR( "Shots missed - %f" ), shotsMissed );
            ImGui::Text( XORSTR( "LBY - %f" ), lby );
        }
        
        ImGui::End();
    }
}