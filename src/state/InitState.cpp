#include <state/InitState.h>
#include <game/RunState.h>
#include <state/LevelState.h>
#include <state/OverworldState.h>


void InitState::updateImpl(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto runState = RunState::generateNewRun();

    // LEVEL START
    
    
//    runState->serialize();
    
    // OVERWORLD START
//    pushState<OverworldState>( runState );

    pushState<LevelState>(m_lua, runState, "arena");
}

InitState::InitState(LuaState &luaState) : GameState(luaState)
{

}
