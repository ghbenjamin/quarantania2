#include <controllers/Controller.h>


Controller::Controller()
        : m_shouldPopController(false) {}


bool Controller::input(IEvent &evt)
{
    return false;
}

void Controller::update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{

}

bool Controller::onMouseMove(IEventMouseMove evt)
{
    return false;
}

bool Controller::onMouseDown(IEventMouseDown evt)
{
    return false;
}

bool Controller::onMouseUp(IEventMouseUp evt)
{
    return false;
}

bool Controller::onKeyDown(IEventKeyPress evt)
{
    return false;
}


bool Controller::hasNextController() const
{
    return m_nextController != std::shared_ptr<Controller>();
}

std::shared_ptr<Controller> Controller::getNextController()
{
    return m_nextController;
}

bool Controller::shouldPopController() const
{
    return m_shouldPopController;
}

void Controller::popController()
{
    m_shouldPopController = true;
}

void Controller::onExit()
{
    m_shouldPopController = false;
    m_nextController = std::shared_ptr<Controller>();
    onExitSelf();
}

void Controller::onEnter()
{
    m_shouldPopController = false;
    m_nextController = std::shared_ptr<Controller>();
    onEnterSelf();
}

void Controller::onEnterSelf() { }
void Controller::onExitSelf() { }

