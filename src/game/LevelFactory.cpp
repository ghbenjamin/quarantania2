#include <game/LevelFactory.h>

LevelPtr LevelFactory::create(LevelConfig const &config, LevelContextPtr const &ctx)
{
    // Broad debug code for now follows:


    ImmutableLevelData imd;

    imd.tilePixelSize = 16;

    imd.levelSize = {10, 10};
    imd.tileCount = imd.levelSize.x() * imd.levelSize.y();

    imd.defaultVisibility = false;
    imd.defaultPassibility = 0;
    imd.defaultLightLevel = 1;

    // Construct tilemap
    imd.tileMap.addTile({"kenney-tiles", "grass-1"}, true);
    imd.tileMap.addTile({"kenney-tiles", "soil-1"}, true);

    // Constuct actual tiles

    // Assume that tileref 0 is the default tile - construct our vector to be all default
    imd.mapLayout = std::vector<TileRef>( imd.tileCount, 0 );

    // TODO actual level contstruction



    auto ptr = std::make_unique<Level>( std::move(imd), ctx );
    return std::move(ptr);
}
