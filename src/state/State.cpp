#include <state/State.h>

bool GameState::hasNextState() const
{
    return !!m_nextState;
}

GameStatePtr GameState::getNextState()
{
    return std::move(m_nextState);
}

bool GameState::hasRequestedPopState() const
{
    return m_requestedPopState;
}

void GameState::requestPopState()
{
    m_requestedPopState = true;
}

GameState::GameState()
: m_requestedPopState(false)
{
}
