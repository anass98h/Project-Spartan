#pragma once

#include <memory>
#include <unordered_map>
#include <sys/mman.h>
#include <link.h>
#include "Utils/patternfinder.h"
#include "Utils/recvproxyhook.h"
#include "SDK/SDK.h"
#include "Utils/vmt.h"
#include "Utils/util.h"
#include "glhook.h"
#include "interfaces.h"

#define LIB_CLIENT_CLIENT XORSTR("client_client.so")
#define LIB_ENGINE_CLIENT XORSTR("engine_client.so"
#define LIB_LAUNCHER_CLIENT XORSTR("launcher_client.so")
#define LIB_VGUIMATSURFACE XORSTR("vguimatsurface_client.so")

struct dlinfo_t {
    const char* library = nullptr;
    uintptr_t address = 0;
    size_t size = 0;
};

namespace Hooker {
    bool GetLibraryInformation( const char* library, uintptr_t* address, size_t* size );

    void InitializeVMHooks();

    bool HookRecvProp( const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook );

    void FindIClientMode();

    void FindGlobalVars();

    void FindCInput();

    void FindGlowManager();

    void FindPlayerResource();

    void FindGameRules();

    void FindRankReveal();

    void FindSendClanTag();

    void FindViewRender();

    void FindSendPacket();

    void FindPrediction();

    void FindIsReadyCallback();

    void FindSurfaceDrawing();

    void FindGetLocalClient();

    void FindLineGoesThroughSmoke();

    void FindInitKeyValues();

    void FindLoadFromBuffer();

    //void FindVstdlibFunctions();
    void FindOverridePostProcessingDisable();

    void FindCrosshairWeaponTypeCheck();

    void FindCamThinkSvCheatsCheck();

    void HookSwapWindow();

    void HookPollEvent();

    void FindSDLInput();

    void FindSetNamedSkybox();
}
