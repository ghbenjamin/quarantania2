#include <game/OverworldFactory.h>

#include <unordered_set>

#include <game/Overworld.h>
#include <game/RunState.h>

OverworldFactory::OverworldFactory() {}

std::unique_ptr<Overworld> OverworldFactory::createOverworld(std::shared_ptr<RunState> runState)
{
    OverworldData data;
    data.gridSize = { 7, 13 };

    std::unordered_set<Vector2i, Vector2iHash> occupiedNodes;
    std::unordered_set<std::pair<Vector2i, Vector2i>, Vector2iPairHash> blockedEdges;

    // Start from the bottom and walk up, creating nodes as we go

    for ( int snode = 0; snode < 4; snode++ )
    {
        int lastCol = runState->randomState.diceRoll(data.gridSize.x()) - 1;
        Vector2i lastLoc = {-1, -1};

        for ( int currRow = 0; currRow < data.gridSize.y(); currRow++ )
        {
            std::vector<int> colOpts;
            for ( int i = -1; i <= 1; i++ )
            {
                int trial = i + lastCol;
                if (trial >= 0 && trial < data.gridSize.x() )
                {
                    auto pair = std::make_pair( lastLoc, Vector2i(trial, currRow) );
                    if ( !blockedEdges.count(pair) )
                    {
                        colOpts.push_back(i);
                    }
                }
            }

            // Select a position to move to at random from the available positions
            int deltaCol = *runState->randomState.randomElement( colOpts );
            int currCol = deltaCol + lastCol;
            Vector2i currLoc = Vector2i{currCol, currRow};

            if (deltaCol == 1)
            {
                blockedEdges.insert( std::make_pair( Vector2i(currCol, currRow - 1), Vector2i(currCol - 1, currRow) ) );
            }
            else if ( deltaCol == -1)
            {
                blockedEdges.insert( std::make_pair( Vector2i(currCol, currRow - 1), Vector2i(currCol + 1, currRow) ) );
            }

            // If this node doesn't already exist, create it
            if ( !occupiedNodes.count(currLoc) )
            {
                OverworldLocation loc;
                loc.type = OverworldLocationType::Mystery;
                loc.displayOffset = {0, 0};
                loc.gridPos = currLoc;

                data.locations.push_back(loc);
                occupiedNodes.insert( currLoc );
            }

            // Add the connection to our list of connections
            if ( lastLoc != Vector2i{-1, -1} )
            {
                data.connections.emplace( lastLoc, currLoc );
            }

            lastLoc = currLoc;
            lastCol = currCol;
        }
    }
    
    return std::make_unique<Overworld>(data, &runState->randomState);
}

