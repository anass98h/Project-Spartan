#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../Hacks/hacks.h"
#include "../SpartanGUI/Spartangui.h"
#include "../hooker.h"

/* Client */
typedef int (* IN_KeyEventFn)( void*, int, int, const char* );
typedef void (* FrameStageNotifyFn)( void*, ClientFrameStage_t );
/* ClientMode */
typedef bool (* ShouldDrawFogFn)( void* );
typedef void (* OverrideViewFn)( void*, CViewSetup* );
typedef bool (* CreateMoveFn)( void*, float, CUserCmd* );
typedef float (* GetViewModelFOVFn)( void* );
/* EngineVGUI */
typedef void (* PaintFn)( void*, PaintMode_t );
/* GameEvents */
typedef bool (* FireEventFn)( void*, IGameEvent*, bool );
typedef bool (* FireEventClientSideFn)( void*, IGameEvent* );
/* GameMovement */
typedef void (* ProcessMovementFn)( void*, C_BasePlayer *player, CMoveData *moveData );
/* Internal Input */
typedef void (* SetKeyCodeStateFn)( void*, ButtonCode_t, bool );
typedef void (* SetMouseCodeStateFn)( void*, ButtonCode_t, MouseCodeState_t );
/* LauncherMgr */
typedef int (* PumpWindowsMessageLoopFn)( void*, void* );
/* Material */
typedef void (* OverrideConfigFn)( void*, MaterialSystem_Config_t*, bool );
typedef void (* BeginFrameFn)( void*, float );
/* ModelRender */
typedef void (* DrawModelExecuteFn)( void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t* );
/* Panel */
typedef void (* PaintTraverseFn)( void*, VPANEL, bool, bool );
/* Sound */
typedef void (* EmitSound1Fn)( void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int,
                               float, int, int, const Vector*, const Vector*, void*, bool, float, int );

typedef void (* EmitSound2Fn)( void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int,
                               soundlevel_t, int, int, const Vector*, const Vector*, void*, bool, float, int );
/* Surface */
typedef void (* OnScreenSizeChangedFn)( void*, int, int );
typedef void (* PlaySoundFn)( void*, const char* );
/* ViewRender */
typedef void (* RenderViewFn)( void*, CViewSetup&, CViewSetup&, unsigned int, int );
typedef void (* RenderSmokePostViewmodelFn)( void* );

namespace Hooks {
    /* Client */
    int IN_KeyEvent( void* thisptr, int eventcode, int keynum, const char* currentbinding );
    void FrameStageNotify( void* thisptr, ClientFrameStage_t stage );
    /* ClientMode */
    bool ShouldDrawFog( void* thisptr );
    void OverrideView( void* thisptr, CViewSetup* pSetup );
    bool CreateMove( void* thisptr, float flInputSampleTime, CUserCmd* cmd );
    float GetViewModelFOV( void* thisptr );
    /* EngineVGUI */
    void Paint( void* thisptr, PaintMode_t mode );
    /* GameEvents */
    bool FireEvent( void* thisptr, IGameEvent* event, bool bDontBroadcast );
    bool FireEventClientSide( void* thisptr, IGameEvent* event );
    /* GameMovement */
    void ProcessMovement( void* thisptr, C_BasePlayer *player, CMoveData *moveData );
    /* Internal Input */
    void SetKeyCodeState( void* thisptr, ButtonCode_t code, bool bPressed );
    void SetMouseCodeState( void* thisptr, ButtonCode_t code, MouseCodeState_t state );
    /* LauncherMgr */
    int PumpWindowsMessageLoop( void* thisptr, void* unknown );
    /* Material */
    void OverrideConfig( void* thisptr, MaterialSystem_Config_t *config, bool forceUpdate );
    void BeginFrame( void* thisptr, float frameTime );
    /* ModelRender */
    void DrawModelExecute( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                           matrix3x4_t* pCustomBoneToWorld );
    /* Panel */
    void PaintTraverse( void* thisptr, VPANEL vgui_panel, bool force_repaint, bool allow_force );
    /* Sound */
    void EmitSound1( void* thisptr, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry,
                     unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation,
                     int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins,
                     bool bUpdatePositions, float soundtime, int speakerentity );

    void EmitSound2( void* thisptr, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry,
                     unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed,
                     soundlevel_t iSoundLevel, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection,
                     void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity );
    /* Surface */
    void OnScreenSizeChanged( void* thisptr, int oldwidth, int oldheight );
    void PlaySound( void* thisptr, const char* filename );
    /* ViewRender */
    void RenderView( void* thisptr, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw );
    void RenderSmokePostViewmodel( void* thisptr );


    /* Cheat-Related / Other */
    void PaintImGui(); // Draw with ImGui.
}

namespace CreateMove {
    extern bool sendPacket;
    extern QAngle lastTickViewAngles;
}

namespace OverrideView {
    extern float currentFOV;
}

namespace SetKeyCodeState {
    extern bool shouldListen;
    extern ButtonCode_t* keyOutput;
}
