#include <game/OverworldFactory.h>
#include <game/Overworld.h>

OverworldFactory::OverworldFactory() {}

std::unique_ptr<Overworld> OverworldFactory::createOverworld(std::shared_ptr<RunState> runState)
{
    RandomInterface random( (RandomGenerator (m_rd())) );
    OverworldData data;
    
    std::vector<std::vector<int>> floorIndexes;
    
    data.gridSize = { 7, 13 };
    int currIdx = 0;

    for ( int j = 0; j < data.gridSize.y(); j++ )
    {
        floorIndexes.push_back({});
    
        for ( int i = 0; i < data.gridSize.x(); i++ )
        {
            if ( random.coinflip() )
            {
                continue;
            }
        
            OverworldLocation loc;
            loc.index = currIdx;
            loc.floor = j;
            loc.type = OverworldLocationType::Mystery;
            loc.displayOffset = {0, 0};
            loc.gridPos = {i, j};
            
            data.locations.push_back(loc);
            floorIndexes.back().push_back(currIdx);
            
            currIdx++;
        }
    }
    
    for ( int i = 0; i < data.gridSize.y() - 1; i++ )
    {
        auto& currFloor = floorIndexes[i];
        auto& nextFloor = floorIndexes[i+1];
    
        data.connections.emplace( currFloor.front(), nextFloor.front() );
    }
    

    return std::make_unique<Overworld>(data, RandomGenerator{ m_rd() });
}

