#pragma once

#include "../Hooks/hooks.h"
#include "../interfaces.h"
#include "../settings.h"

namespace Chams {

    extern IMaterial* chamsMaterial;
    extern IMaterial* ignoredChamsMaterial;

    //Hooks
    void DrawModelExecute( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                           matrix3x4_t* pCustomBoneToWorld );
}
