#include <state/InitState.h>
#include <game/RunState.h>
#include <state/LevelState.h>
#include <state/OverworldState.h>


void InitState::updateImpl(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto runState = RunState::generateNewRun();

    // LEVEL START
    setNextState<LevelState>( runState, "arena" );
    
//    runState->serialize();
    
//    // OVERWORLD START
//    setNextState<OverworldState>( runState );
}