#include "project-spartan.h"
 #include "EventListener.h"

//#include "Utils/netvarmanager.h"

static EventListener* eventListener = nullptr;
// The Below Line is Set by the Build script. Keep this on Line 8.
char buildID[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Line Set by build script
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

bool preload;
bool isShuttingDown = false;

void MainThread()
 {
	if( preload )
	{
		while( client == nullptr )
		{
			client = GetInterface<IBaseClientDLL>(XORSTR("./csgo/bin/linux64/client_client.so"), XORSTR( "VClient"));
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
	}

	Interfaces::FindInterfaces();
	//Interfaces::DumpInterfaces();
    
	Hooker::FindSetNamedSkybox();
	Hooker::FindViewRender();
	Hooker::FindSDLInput();
	Hooker::InitializeVMHooks();
	Hooker::FindIClientMode();
	Hooker::FindGlobalVars();
	Hooker::FindCInput();
	Hooker::FindGlowManager();
	Hooker::FindPlayerResource();
	Hooker::FindGameRules();
	Hooker::FindRankReveal();
	Hooker::FindSendClanTag();
	Hooker::FindSendPacket();
	Hooker::FindPrediction();
	//Hooker::FindIsReadyCallback();
	Hooker::FindSurfaceDrawing();
	Hooker::FindGetLocalClient();
	Hooker::FindLineGoesThroughSmoke();
	Hooker::FindInitKeyValues();
	Hooker::FindLoadFromBuffer();
	//Hooker::FindVstdlibFunctions();
	Hooker::FindOverridePostProcessingDisable();
	Hooker::FindCrosshairWeaponTypeCheck();
	Hooker::FindCamThinkSvCheatsCheck();
	Hooker::HookSwapWindow();
	Hooker::HookPollEvent();

	
	

	ModSupport::OnInit();
	
	clientVMT->HookVM((void*) Hooks::IN_KeyEvent, 20);
	clientVMT->HookVM((void*) Hooks::FrameStageNotify, 36);
	clientVMT->ApplyVMT();

	panelVMT->HookVM((void*) Hooks::PaintTraverse, 42);
	panelVMT->ApplyVMT();

	modelRenderVMT->HookVM((void*) Hooks::DrawModelExecute, 21);
	modelRenderVMT->ApplyVMT();

	clientModeVMT->HookVM((void*) Hooks::OverrideView, 19);
	clientModeVMT->HookVM((void*) Hooks::CreateMove, 25);
	clientModeVMT->HookVM((void*) Hooks::GetViewModelFOV, 36);
	clientModeVMT->ApplyVMT();

	gameEventsVMT->HookVM((void*) Hooks::FireEvent, 9);
	gameEventsVMT->HookVM((void*) Hooks::FireEventClientSide, 10);
	gameEventsVMT->ApplyVMT();

	viewRenderVMT->HookVM((void*) Hooks::RenderSmokePostViewmodel, 41);
	viewRenderVMT->ApplyVMT();

	inputInternalVMT->HookVM((void*) Hooks::SetKeyCodeState, 92);
	inputInternalVMT->HookVM((void*) Hooks::SetMouseCodeState, 93);
	inputInternalVMT->ApplyVMT();

	materialVMT->HookVM((void*) Hooks::BeginFrame, 42);
	materialVMT->ApplyVMT();

	surfaceVMT->HookVM((void*) Hooks::PlaySound, 82);
	surfaceVMT->HookVM((void*) Hooks::OnScreenSizeChanged, 116);
	surfaceVMT->ApplyVMT();

	launcherMgrVMT->HookVM((void*) Hooks::PumpWindowsMessageLoop, 19);
	launcherMgrVMT->ApplyVMT();

	engineVGuiVMT->HookVM((void*) Hooks::Paint, 15);
	engineVGuiVMT->ApplyVMT();

	soundVMT->HookVM((void*) Hooks::EmitSound1, 5);
	soundVMT->HookVM((void*) Hooks::EmitSound2, 6);
	soundVMT->ApplyVMT();

	eventListener = new EventListener({ "cs_game_disconnected", "player_connect_full", "player_death", "player_hurt", "switch_team" });

	if (ModSupport::current_mod != ModType::CSCO && Hooker::HookRecvProp("CBaseViewModel", "m_nSequence", SkinChanger::sequenceHook))
		SkinChanger::sequenceHook->SetProxyFunction((RecvVarProxyFn) SkinChanger::SetViewModelSequence);

	//NetVarManager::DumpNetvars();
	Offsets::GetOffsets();

	Fonts::SetupFonts();

	//Settings::LoadSettings();

	srand(time(NULL)); // Seed random # Generator so we can call rand() later

	AntiAim::LuaInit();

	
}
/* Entrypoint to the Library. Called when loading */
int __attribute__((constructor)) Startup()
{
	// Search in Environment Memory for our buildID before purging environ memory
	for(int i = 0; environ[i]; i++)
	{
		if(strstr(environ[i], buildID) != NULL)
		{
			preload = true;
		
		}
	}

	std::thread mainThread(MainThread);
	// The root of all suffering is attachment
	// Therefore our little buddy must detach from this realm.
	// Farewell my thread, may we join again some day..
	mainThread.detach();
		
	return 0;
}
/* Called when un-injecting the library */
void __attribute__((destructor)) Shutdown()
{
	isShuttingDown = true;
	cvar->FindVar(XORSTR("cl_mouseenable"))->SetValue(1);

	SDL2::UnhookWindow();
	SDL2::UnhookPollEvent();
	if( !preload )
	{
		ImGui::Shutdown();
	}

	AntiAim::LuaCleanup();
	Aimbot::XDOCleanup();
	NoSmoke::Cleanup();

	clientVMT->ReleaseVMT();
	panelVMT->ReleaseVMT();
	modelRenderVMT->ReleaseVMT();
	clientModeVMT->ReleaseVMT();
	gameEventsVMT->ReleaseVMT();
	viewRenderVMT->ReleaseVMT();
	inputInternalVMT->ReleaseVMT();
	materialVMT->ReleaseVMT();
	surfaceVMT->ReleaseVMT();
	launcherMgrVMT->ReleaseVMT();
	engineVGuiVMT->ReleaseVMT();
	soundVMT->ReleaseVMT();

	input->m_fCameraInThirdPerson = false;
	input->m_vecCameraOffset.z = 150.f;
	GetLocalClient(-1)->m_nDeltaTick = -1;

	delete eventListener;

	*bSendPacket = true;
	*s_bOverridePostProcessingDisable = false;
	*CrosshairWeaponTypeCheck = 5;
	*CamThinkSvCheatsCheck = 0x74;
	*(CamThinkSvCheatsCheck + 0x1) = 0x64;

	Util::ProtectAddr(bSendPacket, PROT_READ | PROT_EXEC);
	Util::ProtectAddr(CrosshairWeaponTypeCheck, PROT_READ | PROT_EXEC);
	for (ptrdiff_t off = 0; off < 0x2; off++)
		Util::ProtectAddr(CamThinkSvCheatsCheck + off, PROT_READ | PROT_EXEC);

	cvar->ConsoleColorPrintf(ColorRGBA(255, 0, 0), XORSTR("project-spartan Unloaded successfully.\n"));
}
void projectspartan::SelfShutdown()
{
	// Beta Feature.
	// Does not Correctly/Fully Unload yet.
	/*
	void *self = dlopen(NULL, RTLD_NOW | RTLD_NOLOAD);
	if (self == NULL) {
		cvar->ConsoleDPrintf("Error Unloading: %s\nself Addr: %p\n", dlerror(), self);
		return;
	}
	*/
	Shutdown();
	/*
	dlclose(self);
	dlclose(self);
	*/
}
