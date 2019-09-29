#include <actions/OpenAction.h>

OpenAction::OpenAction(Level *level, EntityRef opener, EntityRef target)
 : Action(level), m_opener(opener), m_target(target)
{

}

ActionResult OpenAction::perform()
{
    // TODO Implement
    return { true };
}
