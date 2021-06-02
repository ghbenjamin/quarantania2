#include <ai/EntityTargetingScheme.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>

std::optional<EntityRef> EntityTargetingNearest::getEntity(Level* level, EntityRef actor)
{
    auto const& [posC, actorC] = level->ecs().getComponents<PositionComponent, ActorComponent>(actor);
    
    std::unordered_map<EntityRef, int> distances;
    
    for ( auto ref : level->ecs().entitiesHaving<ActorComponent>() )
    {
        auto const& [pcC, pcPosC] = level->ecs().getComponents<ActorComponent, PositionComponent>(ref);
        if (pcC->actorType != actorC->actorType)
        {
            int dx = posC->tilePosition.x() - pcPosC->tilePosition.x();
            int dy = posC->tilePosition.y() - pcPosC->tilePosition.y();
            distances[ref] = dx * dx + dy * dy;
        }
    }
    
    auto closest = std::min_element(distances.begin(), distances.end(),
        [](auto const& p1, auto const& p2) { return p1.second < p2.second; });
    
    if (closest == distances.end())
    {
        return {};
    }
    else
    {
        return closest->first;
    }
}
