#include "nofog.h"

bool Settings::NoFog::enabled = false;

bool NoFog::ShouldDrawFog( void* thisptr ) {
    if( !Settings::NoFog::enabled )
        return clientModeVMT->GetOriginalMethod<ShouldDrawFogFn>( 18 )( thisptr );

    /* Skybox Fog is separate */
    IMatRenderContext *renderCtx = material->GetRenderContext();
    renderCtx->FogMode( MaterialFogMode_t::MATERIAL_FOG_NONE );
    renderCtx->Release();
    /* Return false for normal fog */
    return false; // uhhh, no Sweetie, don't draw that fog.
}