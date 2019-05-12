#include <game/LevelFactory.h>

LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{
    auto ptr = std::make_unique<Level>();
    return std::move(ptr);
}
