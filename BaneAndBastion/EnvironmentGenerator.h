#pragma once

#include "EnvironmentObject.h"
namespace BaneAndBastion {
    class EnvironmentGenerator 
    {
    public:
        static std::vector<EnvironmentObject*> GenerateForChunk(int cx, int cy);
    };
}