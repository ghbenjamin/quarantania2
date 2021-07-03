#pragma once


#include <cstdint>

// Forward definitions
class RenderInterface;
class InputInterface;
struct IEvent;

class Overworld
{
public:

    Overworld() = default;
    ~Overworld() = default;

    bool input(IEvent &evt);
    void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    
private:
};