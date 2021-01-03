#pragma once

#include <systems/System.h>
#include <game/GameEventDefs.h>

struct ContainerComponent;
class Item;

class ItemSystem : public System, public GameEventSub<ItemSystem>
{
public:
    explicit ItemSystem(Level *parent);
    ~ItemSystem() override = default;

    void operator()(GameEvents::ItemPickup& evt);
    void operator()(GameEvents::ItemDrop& evt);
    void operator()(GameEvents::ItemEquip& evt);
    void operator()(GameEvents::ItemUnequip& evt);

    template <typename T>
    void operator()(T&& t)
    {}

private:

    void eraseItemFromContainer( const std::shared_ptr<ContainerComponent>& container, std::shared_ptr<Item> item );

};