#pragma once

#include <state/State.h>

// Foward definitions
class Overworld;
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

};