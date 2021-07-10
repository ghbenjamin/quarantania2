#pragma once

#include <state/State.h>

struct LevelContext;
class RunState;

class InitState : public GameState
{

public:
    InitState();
    ~InitState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:
    std::shared_ptr<RunState> generateNewRun();
};