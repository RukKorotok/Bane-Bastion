#pragma once

#include <iostream>

#include <cstdint>
#include <vector>
#include "GameSettings.h"

namespace BaneAndBastion
{

    enum class CellType : std::uint8_t
    {
        Empty = 0,
        Wall = 1,
        Lava = 2,
        Ice = 3
    };

    struct Cell {
        CellType type = CellType::Empty;
        std::uint32_t entityID = 0;

        bool IsPassable() const {
            return type == CellType::Empty && entityID == 0;
        }

        bool CanBeOccupiedBy(std::uint32_t seekerID) const {
            if (type != CellType::Empty)
            {
                return false;
            }
            return (entityID == 0 || entityID == seekerID);
        }
    };

    struct Chunk
    {
        Cell cells[GameSettings::ChunkSize][GameSettings::ChunkSize];
        int biomeID = 0;
    };
}