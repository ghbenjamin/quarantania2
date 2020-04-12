#include <components/ActorComponent.h>

ActorComponent::ActorComponent(RawCreatureData &&data)
: character(data), initiative(200), energyPerTick(10), currentEnergy(0), maxEnergy(10)
{

}

ActorComponent::ActorComponent(PlayerData const &pdata)
: character(pdata), initiative(200), energyPerTick(10), currentEnergy(0), maxEnergy(10)
{

}
