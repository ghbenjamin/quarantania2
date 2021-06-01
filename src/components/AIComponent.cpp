#include <components/AIComponent.h>
#include <ai/BehaviourFactory.h>

AIComponent::AIComponent()
    : behaviour( BehaviourFactory::inertBehaviour() ) {}
