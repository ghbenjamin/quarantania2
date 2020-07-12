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

protected:

    template <typename T, typename... Args>
    void setNextState(Args... args)
    {
        auto dptr = std::make_unique<T>(std::forward<Args>(args)...);
        m_nextState = std::unique_ptr<GameState>( dptr.release() );
    }

    void requestPopState();

private:
    GameStatePtr m_nextState;
    bool m_requestedPopState;

};


