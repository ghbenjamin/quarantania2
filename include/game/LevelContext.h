#pragma once

#include <memory>


struct LevelContext
{
    int depth = 0;
};

using LevelContextPtr = std::shared_ptr<LevelContext>;