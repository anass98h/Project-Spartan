#include "hooks.h"

bool CreateMove::sendPacket = true;
QAngle CreateMove::lastTickViewAngles = QAngle( 0, 0, 0 );

bool Hooks::CreateMove( void* thisptr, float flInputSampleTime, CUserCmd* cmd ) {
    clientModeVMT->GetOriginalMethod<CreateMoveFn>( 25 )( thisptr, flInputSampleTime, cmd );

    if ( cmd && cmd->command_number ) {
        *bSendPacket = CreateMove::sendPacket;
        CreateMove::sendPacket = true;

        lbyindicator::CreateMove( cmd );
        Resolver::CreateMove( cmd );
        BHop::CreateMove( cmd );
        AutoStrafe::CreateMove( cmd );
        ShowRanks::CreateMove( cmd );
        AutoDefuse::CreateMove( cmd );
        JumpThrow::CreateMove( cmd );
        GrenadeHelper::CreateMove( cmd );
        GrenadePrediction::CreateMove( cmd );
        EdgeJump::PrePredictionCreateMove( cmd );

        PredictionSystem::StartPrediction( cmd );
        Autoblock::CreateMove( cmd );
        Aimbot::CreateMove( cmd );
        Triggerbot::CreateMove( cmd );
        AutoKnife::CreateMove( cmd );
        AntiAim::CreateMove( cmd );
        Airstuck::CreateMove( cmd );
        Fakewalk::CreateMove( cmd );
        CircleStrafe::CreateMove( cmd );
        FakeLag::CreateMove( cmd );
        ESP::CreateMove( cmd );
        PredictionSystem::EndPrediction();
        TracerEffect::CreateMove( cmd );

        AngleIndicator::PostPredictionCreateMove( cmd );

        EdgeJump::PostPredictionCreateMove( cmd );

        if ( CreateMove::sendPacket )
            CreateMove::lastTickViewAngles = cmd->viewangles;
    }

    return false;
}
