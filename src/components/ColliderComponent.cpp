#include <components/ColliderComponent.h>

ColliderComponent::ColliderComponent()
: blocksLight(true), blocksMovement(true)
{
}

ColliderComponent::ColliderComponent(bool bl, bool bm)
: blocksLight(bl), blocksMovement(bm)
{

}
