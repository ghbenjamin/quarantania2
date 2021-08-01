#pragma once

#include <memory>
#include <random>


// Forward definitions
class Overworld;
class RunState;

class OverworldFactory
{
public:
    OverworldFactory();
    virtual ~OverworldFactory() = default;

    std::shared_ptr<Overworld> createOverworld(std::shared_ptr<RunState> runState);
};