#pragma once

#include <memory>

// Forward definitions
class RenderInterface;
class InputInterface;
class GameState;
struct IEvent;

// Typedefs
using GameStatePtr = std::unique_ptr<GameState>;


class GameState
{
public:

    GameState();
    virtual ~GameState() = default;

    virtual bool input(IEvent& evt) = 0;
    virtual void update(uint32_t ticks, InputInterface&, RenderInterface&) = 0;

    bool hasNextState() const;
    GameStatePtr getNextState();

    bool hasRequestedPopState() const;
    bool hasRequestedReplaceState() const;
    bool hasRequestedExit() const;

protected:

    // Request that a new state be pushed onto the stack
    template <typename T, typename... Args>
    void setNextState(Args... args)
    {
        auto dptr = std::make_unique<T>(std::forward<Args>(args)...);
        m_nextState = std::unique_ptr<GameState>( dptr.release() );
    }
    
    // Request that the current state be replaced by a new state
    template <typename T, typename... Args>
    void replaceNextState(Args... args)
    {
        setNextState<T>(args...);
        m_requestedReplaceState = true;
    }

    // Request that the current state be popped
    void requestPopState();

    // Request that the program exit.
    void requestExit();

private:
    GameStatePtr m_nextState;
    bool m_requestedReplaceState;
    bool m_requestedPopState;
    bool m_requestedExit;
};


