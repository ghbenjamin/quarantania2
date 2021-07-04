#pragma once

#include <memory>
#include <random>


// Forward definitions
class Overworld;

class OverworldFactory
{
public:
    OverworldFactory();
    virtual ~OverworldFactory() = default;

    std::unique_ptr<Overworld> createOverworld();

private:
    std::random_device m_rd;
};