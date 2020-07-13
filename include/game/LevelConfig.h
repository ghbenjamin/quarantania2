#pragma once

#include <utils/Containers.h>


struct FixedLevelConfig
{
    std::string levelName;
};

struct RandomLevelConfig
{
    Vector2i size;
    int roomDensity = -1;
};