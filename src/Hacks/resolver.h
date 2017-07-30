#pragma once

#include "../settings.h"
#include "../interfaces.h"

namespace Resolver
{
	extern std::vector<int64_t> Players;

	//Hooks
	void FrameStageNotify(ClientFrameStage_t stage);
	void PostFrameStageNotify(ClientFrameStage_t stage);
	void FireGameEvent(IGameEvent* event);
	bool IsEntityMoving(C_BasePlayer* ent);
	bool& LowerBodyYawChanged(C_BasePlayer* ent);
	void StoreVars(C_BasePlayer* ent);
	void StoreVars(C_BasePlayer* ent, QAngle ang, float lby, float simtime, float tick);
	bool& BacktrackThisTick(C_BasePlayer* ent);
	

}



