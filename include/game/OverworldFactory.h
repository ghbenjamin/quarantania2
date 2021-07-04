#pragma once

#include <memory>


// Forward definitions
class Overworld;

class OverworldFactory
{
public:
    OverworldFactory() = default;
    virtual ~OverworldFactory() = default;

    std::unique_ptr<Overworld> createOverworld();

private:
};