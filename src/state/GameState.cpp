#include <state/GameState.h>
#include <ui/lib/Manager.h>


GameState::GameState()
    : m_requestedPopState(false),
      m_requestedReplaceState(false),
      m_requestedExit(false)
{
    m_ui = std::make_shared<UI::Manager>();
}

bool GameState::input( IEvent &evt )
{
    if ( m_ui->input(evt) )
    {
        return true;
    }
    else
    {
        return inputImpl(evt);
    }
}

void GameState::update( uint32_t ticks, InputInterface& iinter, RenderInterface& rinter )
{
    updateImpl(ticks, iinter, rinter);
    m_ui->update(ticks, iinter, rinter);
}


bool GameState::hasNextState() const
{
    return !!m_nextState;
}

std::unique_ptr<GameState> GameState::getNextState()
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

bool GameState::inputImpl( IEvent &evt ) { return false; }
void GameState::updateImpl( uint32_t ticks, InputInterface &, RenderInterface & ) {}

UI::Manager *GameState::ui()
{
    return m_ui.get();
}
