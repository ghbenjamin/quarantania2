#pragma once

#include <memory>

// Forward definitions
class RenderInterface;
class InputInterface;
class GameState;
class LuaState;
struct IEvent;
namespace UI { class Manager; }

class GameState
{
public:

    GameState( LuaState& luaState );
    virtual ~GameState() = default;

    bool input(IEvent& evt);
    void update(uint32_t ticks, InputInterface&, RenderInterface&);

    UI::Manager* ui();

    std::unique_ptr<GameState> getPushedState();
    bool hasPushedState() const;
    bool hasPoppedState() const;
    bool hasRequestedExit() const;

protected:

    virtual bool inputImpl(IEvent& evt);
    virtual void updateImpl(uint32_t ticks, InputInterface&, RenderInterface&);


    // Request that a new state be pushed onto the stack
    template <typename T, typename... Args>
    void pushState(Args&&... args)
    {
        auto dptr = std::make_unique<T>(std::forward<Args>(args)...);
        m_nextState = std::unique_ptr<GameState>( dptr.release() );
    }

    // Request that the current state be popped
    void popState();

    // Request that the program exit.
    void requestExit();
    
    
    std::shared_ptr<UI::Manager> m_ui;
    LuaState& m_lua;

private:

    std::unique_ptr<GameState> m_nextState;
    bool m_requestedPopState;
    bool m_requestedExit;
};


