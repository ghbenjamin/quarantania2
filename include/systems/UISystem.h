#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <resource/Sprite.h>

class UISystem : public System,
                 public GEventSub<GameEvents::LevelReady>,
                 public GEventSub<GameEvents::EntityMove>,
                 public GEventSub<GameEvents::ItemEquip>,
                 public GEventSub<GameEvents::ItemUnequip>,
                 public GEventSub<GameEvents::ItemPickup>,
                 public GEventSub<GameEvents::ItemDrop>,
                 public GEventSub<GameEvents::TurnChange>,
                 public GEventSub<GameEvents::ControllerEntitySelected>
{
public:
    explicit UISystem(Level *parent);
    ~UISystem() override = default;

    void accept(GameEvents::LevelReady  *evt) override;
    void accept(GameEvents::EntityMove  *evt) override;
    void accept(GameEvents::ItemPickup  *evt) override;
    void accept(GameEvents::ItemDrop    *evt) override;
    void accept(GameEvents::ItemEquip   *evt) override;
    void accept(GameEvents::ItemUnequip *evt) override;
    void accept(GameEvents::TurnChange *evt) override;

    void accept(GameEvents::ControllerEntitySelected *evt) override;
};