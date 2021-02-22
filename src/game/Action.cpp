#include <game/Action.h>
#include <game/ResourceDatabase.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>


IActionTargeting::IActionTargeting(Level *level, EntityRef actor)
    : m_level(level), m_actor(actor) { }

SingleTileTargeting::SingleTileTargeting(Level* level, EntityRef actor)
        : IActionTargeting(level, actor) { }

std::vector<Vector2i> SingleTileTargeting::pathToTile(Vector2i tile)
{
    return std::vector<Vector2i>();
}

SingleEntityTargeting::SingleEntityTargeting(Level* level, EntityRef actor)
        : IActionTargeting(level, actor) { }


ActionMoveParent::ActionMoveParent(Level* level, EntityRef actor, int range)
: SingleTileTargeting(level, actor), m_range(range)
{
    // Get & cache the tile that the selected entity could move to
    auto position = m_level->ecs().getComponents<PositionComponent>(m_actor);
    m_pathMap = m_level->grid().allPathsFromTile(position->tilePosition, m_range);
}

bool ActionMoveParent::isMovement() const
{
    return true;
}

GridRegion ActionMoveParent::getValidTiles()
{
    GridRegion gr;
    for (auto const&[k, v] : m_pathMap )
    {
        gr.push_back(k);
    }

    return std::move(gr);
}

bool ActionMoveParent::tileIsValid(Vector2i tile)
{
    return m_pathMap.find(tile) != m_pathMap.end();
}

std::vector<Vector2i> ActionMoveParent::pathToTile(Vector2i tile)
{
    return m_level->grid().pathFromPathMap(m_pathMap, tile);
}

