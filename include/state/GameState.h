#pragma once

#include <memory>

// Forward definitions
class RenderInterface;
class InputInterface;
class GameState;
struct IEvent;
namespace UI { class Manager; }

class GameState
{
public:

    GameState();
    virtual ~GameState() = default;

    bool input(IEvent& evt);
    void update(uint32_t ticks, InputInterface&, RenderInterface&);

    bool hasNextState() const;
    std::unique_ptr<GameState> getNextState();

    bool hasRequestedPopState() const;
    bool hasRequestedReplaceState() const;
    bool hasRequestedExit() const;

protected:

    virtual bool inputImpl(IEvent& evt);
    virtual void updateImpl(uint32_t ticks, InputInterface&, RenderInterface&);


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
    
    
    std::shared_ptr<UI::Manager> m_ui;

private:
    
    std::unique_ptr<GameState> m_nextState;
    bool m_requestedReplaceState;
    bool m_requestedPopState;
    bool m_requestedExit;
};


