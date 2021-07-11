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

    std::unique_ptr<Overworld> createOverworld(std::shared_ptr<RunState> runState);

private:
    std::random_device m_rd;
};