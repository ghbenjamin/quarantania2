#pragma once

#include <game/Level.h>
#include <game/LevelData.h>
#include <game/LevelConfig.h>

class LevelFactory
{
public:
    LevelFactory() = default;
    virtual ~LevelFactory() = default;

    virtual std::unique_ptr<Level> createLevel() = 0;

protected:
    LevelPtr m_level;
    LevelLayout m_levelLayout;
};