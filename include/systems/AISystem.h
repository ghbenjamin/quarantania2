#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>

class AISystem : public System, public GameEventSub<AISystem>
{
public:
    explicit AISystem(Level *parent);
    ~AISystem() override = default;
    
    void update( uint32_t ticks, RenderInterface &rInter ) override;
    
    void operator()(GameEvents::TurnChange& evt);

    template <typename T>
    void operator()(T&& t)
    {}


private:
    std::vector<EntityRef> m_entsToAct;
    std::size_t m_currEnt;
    std::size_t m_passedCount;

};