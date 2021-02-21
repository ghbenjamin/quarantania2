#include <game/Action.h>
#include <game/ResourceDatabase.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <game/GameEventDefs.h>
#include <components/ActorComponent.h>

Action::Action(Level* level, std::string const& id)
        : m_data( ResourceDatabase::instance().actionFromId(id) ),
          m_level(level) {}


std::string const &Action::getName() const
{
    return m_data.name;
}

std::string const &Action::getDescription() const
{
    return m_data.description;
}

SpritesheetKey const &Action::getSprite() const
{
    return m_data.sprite;
}

bool Action::getProvokes() const
{
    return m_data.provokes;
}

void Action::setEnabled(bool val)
{
    m_enabled = val;
}

bool Action::isEnabled() const
{
    return m_enabled;
}

RawActionDataType Action::getType() const
{
    return m_data.type;
}



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

GameAction::GameAction(const Action &data, const std::shared_ptr<IActionTargeting> &impl, TargetingType ttype)
        : data(data), impl(impl), ttype(ttype)
{}

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

