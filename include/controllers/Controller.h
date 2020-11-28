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

    bool hasNextController() const;
    std::shared_ptr<Controller> getNextController();
    bool shouldPopController() const;

protected:

    void setNextController(std::shared_ptr<Controller> const& next);
    void popController();

    virtual bool onMouseMove(IEventMouseMove evt);
    virtual bool onMouseDown(IEventMouseDown evt);
    virtual bool onMouseUp(IEventMouseUp evt);
    virtual bool onKeyDown(IEventKeyPress evt);

private:
    std::shared_ptr<Controller> m_nextController;
    bool m_shouldPopController;
};