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

GameEvents::ItemPickup::ItemPickup(EntityRef actor, EntityRef item)
    : actor(actor), item(item) {}

GameEvents::ItemDrop::ItemDrop(EntityRef actor, std::shared_ptr<Item> item)
    : actor(actor), item(item) {}

GameEvents::ItemEquip::ItemEquip(EntityRef actor, std::shared_ptr<Item> item, CreatureEquipSlot slot)
    : actor(actor), item(item), slot(slot) {}

GameEvents::ItemUnequip::ItemUnequip(EntityRef actor, CreatureEquipSlot slot)
    : actor(actor), slot(slot) {}

GameEvents::EntityDeath::EntityDeath(EntityRef actor)
    : actor(actor) {}

GameEvents::EntityDamage::EntityDamage(EntityRef target, EntityRef source, Damage const& dmg)
    : target(target), source(source), damage(dmg) {}

GameEvents::EntityAction::EntityAction(EntityRef entity, GameAction speed)
    : entity(entity), speed(speed) {}

GameEvents::ControllerEntitySelected::ControllerEntitySelected(EntityRef entity)
    : entity(entity) {}

GameEvents::CombatMeleeAttack::CombatMeleeAttack(EntityRef attacker, EntityRef defender)
    : attacker(attacker), defender(defender) {}

GameEvents::CombatMissedAttack::CombatMissedAttack(EntityRef attacker, EntityRef defender)
    : attacker(attacker), defender(defender) {}

GameEvents::TurnChange::TurnChange(bool isPlayerTurn)
    : isPlayerTurn(isPlayerTurn) {}

GameEvents::CombatAttackSucceeded::CombatAttackSucceeded( EntityRef attacker, EntityRef defender )
    : attacker(attacker), defender(defender) {}
