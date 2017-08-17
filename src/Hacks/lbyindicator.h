#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/entity.h"
#include "antiaim.h"

namespace lbyindicator {
    //Hooks
    void PaintImGui( );

    void CreateMove( CUserCmd *cmd );

}
