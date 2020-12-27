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


ActionMoveStride::ActionMoveStride(Level * level, EntityRef actor, int range)
    : ActionMoveParent(level, actor, range)
{

}

void ActionMoveStride::perform(Vector2i tile)
{
    auto origin = m_level->ecs().getComponents<PositionComponent>(m_actor)->tilePosition;
    auto pathMap = m_level->grid().pathFromPathMap(m_pathMap, tile);

    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, origin, tile, pathMap);
    m_level->events().broadcast<GameEvents::EntityAction>(m_actor, m_range);
}

ActionMoveStep::ActionMoveStep(Level* level, EntityRef actor)
    : ActionMoveParent(level, actor, 1)
{
}


void ActionMoveStep::perform(Vector2i tile)
{
    auto origin = m_level->ecs().getComponents<PositionComponent>(m_actor)->tilePosition;
    auto pathMap = m_level->grid().pathFromPathMap(m_pathMap, tile);

    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, origin, tile, pathMap);
    m_level->events().broadcast<GameEvents::EntityAction>(m_actor, m_range);
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

void ActionMeleeAttack::perform(EntityRef target)
{
    m_level->events().broadcast<GameEvents::CombatMeleeAttack>(m_actor, target);
}

bool ActionMeleeAttack::entityIsValid(EntityRef ref)
{
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(ref);
    if ( actorC && (actorC->actorType == ActorType::NPC) )
    {
        auto posDef = m_level->ecs().tryGetComponent<PositionComponent>(ref);
        auto posAtk = m_level->ecs().tryGetComponent<PositionComponent>(m_actor);

        if ( m_level->entityDistance( m_actor, ref ) <= m_reach )
        {
            return true;
        }
    }

    return false;
}

ActionMeleeAttack::ActionMeleeAttack(Level *level, EntityRef actor, float reach)
    : SingleEntityTargeting(level, actor), m_reach(reach)  {}

void ActionPowerAttack::perform(EntityRef target)
{
    // Do the thing!
}

bool ActionPowerAttack::entityIsValid(EntityRef ref)
{
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(ref);
    return ( actorC && (actorC->actorType == ActorType::NPC) );
}

ActionPowerAttack::ActionPowerAttack(Level *level, EntityRef actor, float reach)
    : SingleEntityTargeting(level, actor), m_reach(reach)  {}