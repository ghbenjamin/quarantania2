#pragma once

#include <memory>


class LevelContext
{
    int depth = 1;
};

using LevelContextPtr = std::shared_ptr<LevelContext>;