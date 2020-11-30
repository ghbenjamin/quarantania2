#pragma once

#include <engine/InputInterface.h>

// Forward definitions
class RenderInterface;

class Controller
{
public:

    Controller();
    virtual ~Controller() = default;

    virtual bool input(IEvent &evt);
    virtual void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    void onEnter();
    void onExit();

    bool hasNextController() const;
    std::shared_ptr<Controller> getNextController();
    bool shouldPopController() const;

protected:

    template <typename T>
    void setNextController(std::shared_ptr<T> const& next)
    {
        static_assert( std::is_base_of_v<Controller, T> );
        m_nextController = std::static_pointer_cast<Controller>(next);
    }

    void popController();


    virtual void onEnterSelf();
    virtual void onExitSelf();
    virtual bool onMouseMove(IEventMouseMove evt);
    virtual bool onMouseDown(IEventMouseDown evt);
    virtual bool onMouseUp(IEventMouseUp evt);
    virtual bool onKeyDown(IEventKeyPress evt);

private:
    std::shared_ptr<Controller> m_nextController;
    bool m_shouldPopController;
};