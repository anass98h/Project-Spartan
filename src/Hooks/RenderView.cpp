#include "hooks.h"

void Hooks::RenderView( void* thisptr, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags,
                        int whatToDraw ) {


    viewRenderVMT->GetOriginalMethod<RenderViewFn>( 6 )( thisptr, setup, hudViewSetup, nClearFlags, whatToDraw );
    SpeedIndicator::RenderView(thisptr, setup, hudViewSetup, nClearFlags, whatToDraw);
    /*
    if( !Settings::SpeedIndicator::enabled || !engine->IsInGame() )
        return;
    C_BasePlayer* localPlayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localPlayer || !localPlayer->GetAlive() )
        return;
    CViewSetup rearView = setup;
    rearView.x = rearView.oldX = 0;
    rearView.y = rearView.oldY = 0;
    rearView.width = rearView.oldWidth = (setup.width/2);
    rearView.height = rearView.oldHeight = (setup.height/2);
    rearView.angles = Vector(-setup.angles.x, (setup.angles.y-180), setup.angles.z);
    rearView.m_flAspectRatio = float(rearView.width) / float(rearView.height);


    static ITexture* mirrorTexture = NULL;
    static std::string textureName = XORSTR( "RearView" );
    if( !mirrorTexture ) {
        material->forceBeginRenderTargetAllocation();
        cvar->ConsoleDPrintf("ImageFormat: %d\n", material->GetBackBufferFormat());
        mirrorTexture = material->createFullFrameRenderTarget(textureName.c_str(), rearView.width, rearView.height);
        //material->forceEndRenderTargetAllocation(); // Causes texture handles to go HAM. scrambles textures.
        if( mirrorTexture ) {
            cvar->ConsoleDPrintf("MirrorTexture \"%s\"Set! -(%p)\n", textureName.c_str(), (void*)mirrorTexture);
        }
        else {
            cvar->ConsoleDPrintf("MirrorTexture Not Set.\n");
            return;
        }
    }

    IMatRenderContext *renderCtx = material->GetRenderContext();
    if( !renderCtx )
        return;

    renderCtx->PushRenderTargetAndViewport();
    renderCtx->SetRenderTarget(mirrorTexture);

    viewRenderVMT->GetOriginalMethod<RenderViewFn>( 6 )( thisptr, rearView, hudViewSetup,
                                                         VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_FULL_TARGET | VIEW_CLEAR_OBEY_STENCIL | VIEW_CLEAR_STENCIL,
                                                         0);

    static IMaterial *rearViewMat = NULL;
    if( !rearViewMat )
    {
        std::stringstream materialData;
        materialData << "\"" << "UnlitGeneric" << "\"\n" <<
                     "{\n" <<
                     "\t\"$basetexture\" \"" << textureName.c_str() << "\"\n"
                             "}\n" << std::flush;

        std::string materialName = XORSTR( "KisakSucks" );
        KeyValues* keyValues = new KeyValues( "UnlitGeneric" );

        InitKeyValues( keyValues, "UnlitGeneric" );
        LoadFromBuffer( keyValues, materialName.c_str(), materialData.str().c_str(), nullptr, NULL, nullptr );

        rearViewMat = material->CreateMaterial( materialName.c_str(), keyValues );
        rearViewMat->AlphaModulate(1.0f);

        if( !rearViewMat ){
            cvar->ConsoleDPrintf("Could not Create Rear View Material! (%s)\n", materialName.c_str());
            //renderCtx->Release();
            return;
        }
    }

    renderCtx->PopRenderTargetAndViewport();
    renderCtx->DrawScreenSpaceRectangle(rearViewMat, 0, 0, rearView.width, rearView.height,
                                        0.0f, 0.0f, float(rearView.width), float(rearView.height),
                                        mirrorTexture->GetActualWidth(), mirrorTexture->GetActualHeight(),
                                        NULL, 1, 1);

    renderCtx->Release();

*/
}