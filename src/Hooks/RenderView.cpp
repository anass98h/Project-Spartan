#include "hooks.h"

enum ClearFlags_t
{
	VIEW_CLEAR_COLOR		= 0x1, // 1
	VIEW_CLEAR_DEPTH		= 0x2, // 2
	VIEW_CLEAR_FULL_TARGET	= 0x4, // 4
	VIEW_NO_DRAW			= 0x8, // 8
	VIEW_CLEAR_OBEY_STENCIL = 0x10,// 16 Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL		= 0x20,// 32
};

enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED				= 0,
	RENDERVIEW_DRAWVIEWMODEL			= (1 << 0), // 1
	RENDERVIEW_DRAWHUD					= (1 << 1), // 2
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2), // 4
};

void Hooks::RenderView(void* thisptr, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw)
{

    /** This is for the Rear-View Mirror. Not quite ready yet **/
    /*
	if( inputSystem->IsButtonDown(ButtonCode_t::KEY_F) )
	{
		C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
		if( localplayer && localplayer->GetAlive() )
		{
            CViewSetup backup = setup;
            CViewSetup hudBackup = hudViewSetup;
            setup.width /= 2;
            setup.height /= 2;
            hudViewSetup.width /= 2;
            hudViewSetup.height /= 2;
            float yawTemp = setup.angles.y;
            yawTemp += 180.0f;
            Math::NormalizeYaw(yawTemp);
            setup.angles.y = yawTemp;
            setup.angles.x = -setup.angles.x;

			cvar->ConsoleDPrintf("------------\n&setup %p\n", &setup);
			cvar->ConsoleDPrintf("width(%d), height(%d), X(%f), Y(%f), Z(%f), angX(%f), angY(%f), angZ(%f), fov(%f)\n",
								 setup.width, setup.height, setup.origin.x, setup.origin.y, setup.origin.z, setup.angles.x, setup.angles.y, setup.angles.z, setup.fov);

			cvar->ConsoleDPrintf("&HUDsetup %p\n", &hudViewSetup);
			cvar->ConsoleDPrintf("width(%d), height(%d), X(%f), Y(%f), Z(%f), angX(%f), angY(%f), angZ(%f), fov(%f)\n",
								 hudViewSetup.width, hudViewSetup.height, hudViewSetup.origin.x, hudViewSetup.origin.y, hudViewSetup.origin.z,
								 hudViewSetup.angles.x, hudViewSetup.angles.y, hudViewSetup.angles.z, hudViewSetup.fov);
            cvar->ConsoleDPrintf("nClearFlags (%d) - whatToDraw(%d)\n", nClearFlags, whatToDraw);

            // Works but models are in front of the Rear-view
            //viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, setup, hudViewSetup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_FULL_TARGET | VIEW_CLEAR_OBEY_STENCIL | VIEW_CLEAR_STENCIL, 0);
            //viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, backup, hudBackup, 0, RENDERVIEW_DRAWVIEWMODEL | RENDERVIEW_DRAWHUD);

            //Works but Regular view is black
            //viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, backup, hudBackup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_FULL_TARGET | VIEW_CLEAR_OBEY_STENCIL | VIEW_CLEAR_STENCIL,
            //                                                  RENDERVIEW_DRAWVIEWMODEL | RENDERVIEW_DRAWHUD);
            //viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, setup, hudViewSetup, 0, 0);


            viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, setup, hudViewSetup, 0, 0);
            viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, backup, hudBackup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_OBEY_STENCIL | VIEW_CLEAR_STENCIL
                                                                                        , RENDERVIEW_DRAWVIEWMODEL | RENDERVIEW_DRAWHUD);

        }
	}
	else
	{
     */
		viewRenderVMT->GetOriginalMethod<RenderViewFn>(6)(thisptr, setup, hudViewSetup, nClearFlags, whatToDraw);
	//}
}