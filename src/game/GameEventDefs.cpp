#include <game/GameEventDefs.h>

GameEvents::EntityMove::EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos)
    : ent(ent), oldPos(oldPos), newPos(newPos), path(std::nullopt) {}

GameEvents::EntityMove::EntityMove(EntityRef ent, const Vector2i &oldPos,
                                   const Vector2i &newPos, const std::vector<Vector2i> &path)
    : ent(ent), oldPos(oldPos), newPos(newPos), path(path) {}

GameEvents::EntityReady::EntityReady(EntityRef ent)
    : ent(ent) {}

GameEvents::EntityOpenClose::EntityOpenClose(EntityRef ent, bool isOpen)
    : ent(ent), isOpen(isOpen) {}

GameEvents::MeleeAttack::MeleeAttack(EntityRef attacker, EntityRef defender, WeaponPtr const& weapon)
    : attacker(attacker), defender(defender), weapon(weapon) {}

GameEvents::ItemPickup::ItemPickup(EntityRef actor, EntityRef item)
    : actor(actor), item(item) {}

GameEvents::ItemDrop::ItemDrop(EntityRef actor, std::shared_ptr<Item> item)
    : actor(actor), item(item) {}

GameEvents::ItemEquip::ItemEquip(EntityRef actor, std::shared_ptr<Item> item, EquipSlot slot)
    : actor(actor), item(item), slot(slot) {}

GameEvents::ItemUnequip::ItemUnequip(EntityRef actor, EquipSlot slot)
    : actor(actor), slot(slot) {}

GameEvents::EntityDeath::EntityDeath(EntityRef actor)
    : actor(actor) {}

GameEvents::EntityDamage::EntityDamage(EntityRef target, Damage const *dmg)
    : target(target), damage(dmg) {}

GameEvents::TurnChange::TurnChange(EntityRef prev, EntityRef curr)
    : previous(prev), current(curr) {}

GameEvents::EntityAction::EntityAction(EntityRef entity, int actionPoints)
    : entity(entity), actionPoints(actionPoints) {}

GameEvents::ControllerEntitySelected::ControllerEntitySelected(EntityRef entity)
    : entity(entity) {}
