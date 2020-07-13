#include <game/FixedLevelFactory.h>

FixedLevelFactory::FixedLevelFactory()
{

}

LevelPtr FixedLevelFactory::create(const FixedLevelConfig &config, const LevelContextPtr &ctx, const PartyData &pdata)
{
    return LevelPtr();
}
