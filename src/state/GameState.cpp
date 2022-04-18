#include <state/GameState.h>
#include <ui/lib/Manager.h>


GameState::GameState( LuaState& luaState )
    : m_requestedPopState(false),
      m_requestedExit(false),
      m_lua(luaState)
{
    m_ui = std::make_shared<UI::Manager>(m_lua);
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


bool GameState::hasPushedState() const
{
    return !!m_nextState;
}

std::unique_ptr<GameState> GameState::getPushedState()
{
    return std::move(m_nextState);
}

bool GameState::hasPoppedState() const
{
    return m_requestedPopState;
}

void GameState::popState()
{
    m_requestedPopState = true;
}

void GameState::requestExit()
{
    m_requestedExit = true;
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
