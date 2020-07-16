#include <game/FixedLevelFactory.h>

FixedLevelFactory::FixedLevelFactory() {}

LevelPtr FixedLevelFactory::create(TiledMap const& map, const LevelContextPtr &ctx, const PartyData &pdata)
{
    return LevelPtr();
}
