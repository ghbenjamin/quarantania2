#include <state/State.h>


GameState::GameState()
    : m_requestedPopState(false),
      m_requestedReplaceState(false),
      m_requestedExit(false)
{
}


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

void GameState::requestExit()
{
    m_requestedExit = true;
}

bool GameState::hasRequestedReplaceState() const
{
    return m_requestedReplaceState;
}

bool GameState::hasRequestedExit() const
{
    return m_requestedExit;
}
