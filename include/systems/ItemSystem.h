#pragma once

#include <systems/System.h>
#include <game/GEventDefs.h>

class ContainerComponent;
class Item;

class ItemSystem : public System,
                   public GEventSub<GEvents::ItemPickup>,
                   public GEventSub<GEvents::ItemDrop>,
                   public GEventSub<GEvents::ItemEquip>
{
public:
    explicit ItemSystem(Level *parent);
    ~ItemSystem() override = default;

    void accept(GEvents::ItemPickup *evt) override;
    void accept(GEvents::ItemDrop *evt) override;
    void accept(GEvents::ItemEquip *evt) override;

private:

    void eraseItemFromContainer( std::shared_ptr<ContainerComponent> container, std::shared_ptr<Item> item );

};