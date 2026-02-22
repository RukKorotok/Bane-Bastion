#pragma once

#pragma once
#include "EnvironmentObject.h"

namespace BaneAndBastion {
    class Wall : public EnvironmentObject 
    {
    public:
        Wall(FalkonEngine::Vector2Df position);
    };
}
