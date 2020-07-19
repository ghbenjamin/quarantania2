#pragma once

#include <game/Level.h>
#include <game/LevelData.h>
#include <game/LevelConfig.h>

class LevelFactory
{
public:

    LevelFactory();
    virtual ~LevelFactory() = default;

protected:
    LevelPtr m_level;
    LD::LevelLayout m_levelLayout;

    std::random_device m_rd;
};