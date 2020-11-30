#pragma once

#include <systems/System.h>
#include <game/GameEventDefs.h>

struct ContainerComponent;
class Item;

class ItemSystem : public System,
                   public GEventSub<GameEvents::ItemPickup>,
                   public GEventSub<GameEvents::ItemDrop>,
                   public GEventSub<GameEvents::ItemUnequip>,
                   public GEventSub<GameEvents::ItemEquip>
{
public:
    explicit ItemSystem(Level *parent);
    ~ItemSystem() override = default;

    void accept(GameEvents::ItemPickup *evt) override;
    void accept(GameEvents::ItemDrop *evt) override;
    void accept(GameEvents::ItemEquip *evt) override;
    void accept(GameEvents::ItemUnequip *evt) override;

private:

    void eraseItemFromContainer( const std::shared_ptr<ContainerComponent>& container, std::shared_ptr<Item> item );

};