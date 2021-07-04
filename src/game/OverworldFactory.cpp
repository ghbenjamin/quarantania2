#include <game/OverworldFactory.h>
#include <game/Overworld.h>

std::unique_ptr<Overworld> OverworldFactory::createOverworld()
{
    return std::unique_ptr<Overworld>();
}
