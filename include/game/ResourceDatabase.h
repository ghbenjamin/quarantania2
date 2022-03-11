#pragma once

#include <memory>
#include <string_view>

#include <game/RawData.h>
#include <utils/Assert.h>
#include <utils/Random.h>
#include <game/ActorData.h>

class RandomState;

class ResourceDatabase
{
public:

    static ResourceDatabase& get()
    {
        static ResourceDatabase s;
        return s;
    }

    ~ResourceDatabase() = default;

    ItemData itemFromName( std::string_view name );
    WeaponData weaponFromName (std::string_view name );
    ArmourData armourFromName( std::string_view name );
    CreatureData creatureFromName( std::string_view name );
    ObjectData objectFromName( std::string_view name );
    ActionData actionFromId(std::string_view id );
    ModifierData modifierFromId(std::string_view id );
    PlayerData chargenFromClass( std::string_view id );
    
    std::vector<std::string> const& randomNames();

private:
    ResourceDatabase();

    void loadAllCreatureData();
    void loadAllItemData();
    void loadAllObjectData();
    void loadAllActionData();
    void loadAllModifierData();
    void loadAllChargenData();

    std::vector<CreatureData> m_creatureData;
    std::vector<ItemData> m_itemData;
    std::vector<WeaponData> m_weaponData;
    std::vector<ArmourData> m_armourData;
    std::vector<ObjectData> m_objectData;
    std::vector<ActionData> m_actionData;
    std::vector<ModifierData> m_modifierData;
    
    std::vector<PlayerData> m_chargenData;
    std::vector<std::string> m_randomNames;
    
};