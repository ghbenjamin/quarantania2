#include <components/Collider.h>

Components::Collider::Collider()
: blocksLight(true), blocksMovement(true)
{
}

Components::Collider::Collider(bool bl, bool bm)
: blocksLight(bl), blocksMovement(bm)
{

}
