#pragma once

#include <memory>

#include <state/GameState.h>
#include <game/GameEvent.h>

// Foward definitions
class Overworld;
class OverworldStateEventSub;
class RunState;
namespace UI { class Manager; }

class OverworldState : public GameState
{
public:
    OverworldState(LuaState& luaState, std::shared_ptr<RunState>);
    ~OverworldState() override = default;

    Overworld* overworld();

    void startLevel(int idx); // TODO Arguments

protected:
    bool inputImpl(IEvent &evt) override;
    void updateImpl(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:
    void setupUI( );
    
    std::shared_ptr<RunState> m_runState;
    std::shared_ptr<Overworld> m_overworld;
    std::unique_ptr<OverworldStateEventSub> m_eventSub;
};



class OverworldStateEventSub : public GameEventSub<OverworldStateEventSub>
{
public:
    explicit OverworldStateEventSub(OverworldState* parent);
    ~OverworldStateEventSub() override = default;
    
    void operator()(GameEvents::OverworldLocationSelect& evt);
    
    template <typename T>
    void operator()(T&& t)
    {}


private:
    OverworldState* m_parent;
};