#include "resolverinfo.h"

bool ResolverInfo::showWindow = false;

void ResolverInfo::RenderWindow() {
    if ( !ResolverInfo::showWindow )
        return;

    ImGui::SetNextWindowSize( ImVec2( 300, 400 ), ImGuiCond_FirstUseEver );

    if ( ImGui::Begin( XORSTR( "Resolver Info" ) ) ) {
        if ( !engine->IsInGame() ) {
            ImGui::Text( XORSTR( "Not in-game" ) );
        } else {
            int playerId = Resolver::resolvingId;
    
            for ( int i = 1; i < engine->GetMaxClients(); i++ ) {
                if ( playerId == i ) {
                    IEngineClient::player_info_t playerInfo;
                    engine->GetPlayerInfo( i, &playerInfo );
    
                    char* name = playerInfo.name;
                    char* guid = playerInfo.guid;
    
                    float lastHit = Resolver::lastHitAng[i];
                    float angForce = Resolver::angForce[i];
                    const char* angForceTxt = Resolver::angForceTxt[i];
                    int shotsMissed = Resolver::shotsMiss[i];
                    float lby = Resolver::lby;
    
                    ImGui::Text( "%s [%s]\n", name, guid );
                    ImGui::Text( "Last hit - %i\n", lastHit );
                    ImGui::Text( "Trying Angle - %s [%i]\n", angForceTxt, angForce );
                    ImGui::Text( "Last Hit - %i\n", lastHit );
                    ImGui::Text( "Shots missed - %i\n", shotsMissed );
                    ImGui::Text( "LBY - %i\n", lby );
                }
            }
        }   
        
        ImGui::End();
    }
}