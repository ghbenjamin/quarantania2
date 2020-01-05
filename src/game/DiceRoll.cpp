#include <game/DiceRoll.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

DiceRoll DiceRoll::parseFromString(std::string const &data)
{
    DiceRoll roll;
    std::smatch match;

    if ( std::regex_search( data, match, DiceRollRegex ) && match.size() > 1 )
    {

        roll.diceCount = std::atoi( match.str(1).c_str() );
        roll.diceSize = std::atoi( match.str(2).c_str() );

        Logging::log( match.size() );

        if ( match.size() == 5 )
        {
            int parity = 1;
            if ( match.str(3) == "-" )
            {
                parity = -1;
            }
            else if ( match.str(3) != "+" )
            {
                AssertAlways();
            }

            roll.modifier = std::atoi( match.str(4).c_str() );
            roll.modifier *= parity;
        }
    }
    else
    {
        // Bad attack string 
        AssertAlways();
    }

    return roll;
}
