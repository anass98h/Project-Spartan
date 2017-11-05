#pragma once

#include "../interfaces.h"
#include "../settings.h"
#include "../SDK/SDK.h"

namespace ThirdPerson {

    // Hooks

    void BeginFrame();

    void FrameStageNotify( ClientFrameStage_t stage );

    void DrawModelExecute( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                           matrix3x4_t* pCustomBoneToWorld );

}
