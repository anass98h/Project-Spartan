
#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Hooks/hooks.h"
#include "../interfaces.h"
#include "aimbot.h"
#include <lua.hpp>

namespace AntiAim
{
	extern bool isAntiAiming;
	extern float lastRealYaw;
	extern float lastFakeYaw;
	extern float lastPitch;
	void LuaInit();
	void LuaCleanup();

	//Hooks
	void CreateMove(CUserCmd* cmd);
}
