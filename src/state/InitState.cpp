#include <state/InitState.h>
#include <game/LevelContext.h>
#include <game/RunState.h>
#include <state/LevelState.h>
#include <state/OverworldState.h>

InitState::InitState() {}

bool InitState::input(IEvent &evt)
{
    return false;
}

void InitState::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto runState = RunState::generateNewRun();

    // LEVEL START
    auto ctx = std::make_shared<LevelContext>();
    setNextState<LevelState>( ctx, runState );
    
//    // OVERWORLD START
//    setNextState<OverworldState>( runState );
}