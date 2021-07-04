#include <game/OverworldFactory.h>
#include <game/Overworld.h>

OverworldFactory::OverworldFactory()
{

}


std::unique_ptr<Overworld> OverworldFactory::createOverworld()
{
    RandomInterface random( (RandomGenerator (m_rd())) );
    OverworldData data;

    data.gridSize = { 7, 13 };
    
    
    for ( int i = 0; i < data.gridSize.x(); i++ )
    {
        for ( int j = 0; j < data.gridSize.y(); j++ )
        {
            if ( random.coinflip() )
            {
                continue;
            }
        
            OverworldLocation loc;
            loc.type = OverworldLocationType::Mystery;
            loc.displayOffset = {0, 0};
            loc.gridPos = {i, j};
            
            data.locations.push_back(loc);
        }
    }

    return std::make_unique<Overworld>(data, RandomGenerator{ m_rd() });
}

