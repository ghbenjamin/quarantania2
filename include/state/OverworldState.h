#pragma once

#include <memory>

#include <state/State.h>
#include <game/GameEvent.h>

// Foward definitions
class Overworld;
class OverworldStateEventSub;
namespace UI { class Manager; }

class OverworldState : public GameState
{
public:
    
    OverworldState();
    ~OverworldState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:
    void setupUI( );

    std::unique_ptr<Overworld> m_overworld;
    std::unique_ptr<UI::Manager> m_ui;
    std::unique_ptr<OverworldStateEventSub> m_eventSub;
};





class OverworldStateEventSub : public GameEventSub<OverworldStateEventSub>
{
public:
    explicit OverworldStateEventSub(Overworld* parent);
    ~OverworldStateEventSub() override = default;
    
    void operator()(GameEvents::OverworldLocationSelect& evt);
    
    template <typename T>
    void operator()(T&& t)
    {}


private:
    Overworld* m_overworld;
};