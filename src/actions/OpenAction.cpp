#include <actions/OpenAction.h>

OpenAction::OpenAction(Level *level, EntityRef entity)
        : EntityAction(level, entity)
{

}

bool OpenAction::doAction(EntityRef actor) const
{
    // NOOP for now
    return true;
}
