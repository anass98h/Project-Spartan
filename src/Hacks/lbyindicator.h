#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/entity.h"

namespace lbyindicator
{
	//Hooks
    void Paint();
    void CreateMove(CUserCmd* cmd);
    
}