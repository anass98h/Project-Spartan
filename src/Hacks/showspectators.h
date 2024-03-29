#pragma once

#include "../settings.h"
#include "../interfaces.h"
#include "../Utils/entity.h"

namespace ShowSpectators {
    std::list<int> GetObservervators( int playerId );

    void RenderWindow();
}
