#include "hooks.h"

float OverrideView::currentFOV = 90.0f;

void Hooks::OverrideView( void* thisptr, CViewSetup* pSetup ) {
    if ( !Settings::ScreenshotCleaner::enabled || !engine->IsTakingScreenshot() ) {
        GrenadePrediction::OverrideView( pSetup );
        FOVChanger::OverrideView( pSetup );
        ThirdPerson::OverrideView( pSetup );
    }

    OverrideView::currentFOV = pSetup->fov;

    clientModeVMT->GetOriginalMethod<OverrideViewFn>( 19 )( thisptr, pSetup );
}
