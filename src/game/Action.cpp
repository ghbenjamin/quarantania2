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

GridRegion SingleEntityTargeting::getValidTiles()
{
    GridRegion validTiles;
    
    for ( auto ref : m_level->ecs().allEntities() )
    {
        if ( entityIsValid(ref) )
        {
            auto pos = m_level->ecs().getComponents<PositionComponent>(ref);
            validTiles.push_back( pos->tilePosition );
        }
    }
    
    return validTiles;
}

void ReifiedGameAction::perform(Level* level, EntityRef actor)
{
    action.impl->attach( level, actor );

    switch (action.ttype)
    {
        case TargetingType::SingleTile:
            std::static_pointer_cast<SingleTileTargeting>(action.impl)->perform( std::get<Vector2i>(target) );
            break;
        case TargetingType::GridRegion:
            AssertNotImplemented();
            break;
        case TargetingType::SingleEntity:
            std::static_pointer_cast<SingleEntityTargeting>(action.impl)->perform( std::get<EntityRef>(target) );
            break;
    }

    level->events().broadcast<GameEvents::EntityAction>(actor, action);
}

ReifiedGameAction::ReifiedGameAction(const GameAction &action, const ActionTarget &target)
        : action(action), target(target)
{}

ActionSpeedData::ActionSpeedData( GameAction const* action )
: action(action), modified(action->data.speed) {}

ActionSpeedData::ActionSpeedData( GameAction *action )
: action(action), modified(action->data.speed) {}
