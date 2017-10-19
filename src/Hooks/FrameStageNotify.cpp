#include "hooks.h"

void Hooks::FrameStageNotify( void* thisptr, ClientFrameStage_t stage ) {

    // Pasted PVS fix from Dr_P3pp3r, Adapted to work on GNU/Linux
    if( stage == ClientFrameStage_t::FRAME_RENDER_START )
    {
        C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
        for ( int i = 1; i < engine->GetMaxClients(); ++i ) {
            C_BasePlayer* player = ( C_BasePlayer* ) entityList->GetClientEntity( i );

            if ( !player
                 || player == localplayer
                 || player->GetDormant()
                 || !player->GetAlive()
                 || player->GetImmune() )
                continue;

            IEngineClient::player_info_t playerInfo;
            engine->GetPlayerInfo(player->GetIndex(), &playerInfo);

            // These were hard as shit to find: https://i.imgur.com/gTqgDLO.png
            *(int*)((uintptr_t)player + 0xFEC) = globalVars->framecount; // Last Occlusion check frame#
            *(int*)((uintptr_t)player + 0xFE4) = 0; // Occlusion flags
        }
    }
    Backtracking::FrameStageNotify( stage );
    CustomGlow::FrameStageNotify( stage );
    SkinChanger::FrameStageNotifyModels( stage );
    SkinChanger::FrameStageNotifySkins( stage );
    Noflash::FrameStageNotify( stage );
    View::FrameStageNotify( stage );
    Resolver::FrameStageNotify( stage );
    SkyBox::FrameStageNotify( stage );
    ASUSWalls::FrameStageNotify( stage );
    NoSmoke::FrameStageNotify( stage );
    ThirdPerson::FrameStageNotify( stage );

    if ( SkinChanger::forceFullUpdate ) {
        GetLocalClient( -1 )->m_nDeltaTick = -1;
        SkinChanger::forceFullUpdate = false;
    }

    clientVMT->GetOriginalMethod<FrameStageNotifyFn>( 36 )( thisptr, stage );

    Resolver::PostFrameStageNotify( stage );
    View::PostFrameStageNotify( stage );
}
