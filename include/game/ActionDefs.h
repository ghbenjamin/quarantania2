#pragma once

//#include <game/Action.h>
//#include <game/Items.h>
//
//class Item;
//
//class StepAction : public TileAction
//{
//public:
//    using TileAction::TileAction;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//};
//
//
//class OpenAction : public EntityAction
//{
//public:
//    using EntityAction::EntityAction;
//
//    bool canTryAction() const override;
//    const char* description() const override;
//    bool doAction() const override;
//};
//
//
//class CloseAction : public EntityAction
//{
//public:
//    using EntityAction::EntityAction;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//};
//
//class ExitLevelAction : public EntityAction
//{
//public:
//    using EntityAction::EntityAction;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//};
//
//
//class MeleeAttackAction : public EntityAction
//{
//public:
//    using EntityAction::EntityAction;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//};
//
//
//class PickUpItemAction : public EntityAction
//{
//public:
//    using EntityAction::EntityAction;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//};
//
//
//class DropItemAction : public Action
//{
//public:
//    DropItemAction(Level* level, EntityRef actor, std::shared_ptr<Item> item);
//    ~DropItemAction() override = default;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//
//private:
//    EntityRef m_actor;
//    std::shared_ptr<Item> m_item;
//};
//
//
//class EquipItemAction : public Action
//{
//public:
//    EquipItemAction(Level* level, EntityRef actor, std::shared_ptr<Item> item);
//    ~EquipItemAction() override = default;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//
//private:
//    EntityRef m_actor;
//    std::shared_ptr<Item> m_item;
//};
//
//class UnequipItemAction : public Action
//{
//public:
//    UnequipItemAction(Level* level, EntityRef actor, EquipSlot slot);
//    ~UnequipItemAction() override = default;
//
//    const char* description() const override;
//    bool canTryAction() const override;
//    bool doAction() const override;
//
//private:
//    EntityRef m_actor;
//    EquipSlot m_slot;
//};
