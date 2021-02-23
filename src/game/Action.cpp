#include <game/Action.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>


IActionTargeting::IActionTargeting()
        : m_level(nullptr), m_actor(EntityNull) {}

void IActionTargeting::attach(Level* level, EntityRef actor)
{
    m_level = level;
    m_actor = actor;

    attachImpl();
}

void IActionTargeting::attachImpl() {}

std::vector<Vector2i> SingleTileTargeting::pathToTile(Vector2i tile)
{
    return std::vector<Vector2i>();
}

