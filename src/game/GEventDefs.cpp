#include <game/GEventDefs.h>

GEvents::EntityMove::EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos)
    : ent(ent), oldPos(oldPos), newPos(newPos) {}

GEvents::EntityReady::EntityReady(EntityRef ent)
    : ent(ent) {}

GEvents::EntityOpenClose::EntityOpenClose(EntityRef ent, bool isOpen)
    : ent(ent), isOpen(isOpen) {}

GEvents::MeleeAttack::MeleeAttack(EntityRef attacker, EntityRef defender)
    : attacker(attacker), defender(defender) {}

GEvents::ItemPickup::ItemPickup(EntityRef actor, EntityRef item)
    : actor(actor), item(item) {}

GEvents::ItemDrop::ItemDrop(EntityRef actor, std::shared_ptr<Item> item)
    : actor(actor), item(item) {}

GEvents::ItemEquip::ItemEquip(EntityRef actor, std::shared_ptr<Item> item, EquipSlot slot)
    : actor(actor), item(item), slot(slot) {}
