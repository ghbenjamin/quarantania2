#pragma once

#include <cstdint>
#include <memory>

class Level;
class IEvent;
class InputInterface;
class RenderInterface;

class LevelController
{
public:
    LevelController( Level* level );
    virtual ~LevelController() = default;

    virtual bool input(IEvent &evt);
    virtual void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

private:

    Level* m_level;
};

using LevelControllerPtr = std::unique_ptr<LevelController>;