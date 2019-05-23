#include <game/LevelFactory.h>

LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{
    LevelInitData lid;
    lid.levelSize = {10, 10};
    lid.defaultVisibility = false;
    lid.defaultPassibility = 0;
    lid.defaultLightLevel = 1;

    auto ptr = std::make_unique<Level>(lid);
    return std::move(ptr);
}
