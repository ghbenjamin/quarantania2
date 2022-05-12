#include <game/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <utils/StringUtils.h>
#include <game/GameParse.h>

ResourceDatabase::ResourceDatabase()
{
    loadAllItemData();
    loadAllCreatureData();
    loadAllActionData();
    loadAllModifierData();
    loadAllChargenData();
}

ItemData ResourceDatabase::itemFromId( std::string_view id )
{
    auto it = std::find_if( m_itemData.begin(), m_itemData.end(),
        [id](auto const& item){ return item.id == id; });

    AssertMsg( it != m_itemData.end(), fmt::format( "Unexpected item: {}", id ) );
    return { *it };
}

WeaponData ResourceDatabase::weaponFromId( std::string_view id )
{
    auto it = std::find_if( m_weaponData.begin(), m_weaponData.end(),
        [id]( auto const& item){ return item.itemId == id; });

    Assert( it != m_weaponData.end() );
    return { *it };
}

CreatureData ResourceDatabase::creatureFromName( std::string_view name)
{
    auto it = std::find_if( m_creatureData.begin(), m_creatureData.end(),
        [name](auto const& item){ return item.name == name; });

    AssertMsg( it != m_creatureData.end(), fmt::format("Unknown creature '{}'", name)  );
    return { *it };
}

ArmourData ResourceDatabase::armourFromId( std::string_view id )
{
    auto it = std::find_if( m_armourData.begin(), m_armourData.end(),
        [id]( auto const& item){ return item.itemId == id; });

    Assert( it != m_armourData.end() );
    return { *it };
}

ActionData ResourceDatabase::actionFromId( std::string_view id)
{
    auto it = std::find_if( m_actionData.begin(), m_actionData.end(),
        [id](auto const& item){ return item.id == id; });

    Assert( it != m_actionData.end() );
    return { *it };
}

ModifierData ResourceDatabase::modifierFromId( std::string_view id )
{
    auto it = std::find_if( m_modifierData.begin(), m_modifierData.end(),
        [id](auto const& item){ return item.id == id; });
    
    Assert( it != m_modifierData.end() );
    return { *it };
}

PlayerData ResourceDatabase::chargenFromClass( std::string_view name )
{
    auto it = std::find_if( m_chargenData.begin(), m_chargenData.end(),
                            [name](auto const& item){ return item.playerClass == name; });
    
    Assert( it != m_chargenData.end() );
    return { *it };
}


void ResourceDatabase::loadAllCreatureData()
{
    sol::table mods = m_lua.runLoadedScript( "data/Creatures" );
    for ( auto const& [k, v] : mods )
    {
        sol::table const& data = v;
        CreatureData rcd;
    
        rcd.name = data["name"];
        rcd.alignment = EnumParse::alignment( data["alignment"] );
        rcd.creatureType = data["creature_type"];
        rcd.sprite = data.get<std::string>( "sprite" );
        rcd.xp = data["xp"];
        rcd.maxHP = data["hp"];
        rcd.attrStr = data["attr_str"];
        rcd.attrDex = data["attr_dex"];
        rcd.attrCon = data["attr_con"];
        rcd.attrInt = data["attr_int"];
        rcd.attrWis = data["attr_wis"];
        rcd.attrCha = data["attr_cha"];
        rcd.saveFort = data["save_fort"];
        rcd.saveRef = data["save_ref"];
        rcd.saveWill = data["save_will"];
        rcd.baseSpeed = data["speed"];
        rcd.description = data.get_or( "description", std::string() );
        rcd.creatureSubtypes = data["subtype"].get_or<std::vector<std::string>>( {} );
        rcd.featIds = data["feats"].get_or<std::vector<std::string>>( {} );
        rcd.immune = data["immune"].get_or<std::vector<std::string>>( {} );
        rcd.damageResistance = data["damage_resistance"].get_or<decltype(rcd.damageResistance)>( {} );
       
        m_creatureData.push_back( std::move(rcd) );
    }
}

void ResourceDatabase::loadAllItemData()
{
    sol::table data = m_lua.runLoadedScript( "data/Items" );
    
    for ( auto const& [k, v] : data )
    {
        sol::table const& item = v;
        ItemData rit;
    
        rit.id = k.as<sol::string_view>();
        rit.name = item["name"];
        rit.weight = item["weight"];
        rit.value = item["price"];
        rit.description = item["description"];
        rit.itemType = item["item_type"];
        rit.sprite = item.get<std::string>( "sprite" );
        rit.slot = item.get_or( "slot", std::string() );
        
        auto vWeapon = item["weapon"];
        if ( vWeapon != sol::nil )
        {
            WeaponData rwd;
            sol::table const& tWeapon = vWeapon;
            
            rwd.itemId = rit.id;
            rwd.critLower = tWeapon["crit_lower"];
            rwd.critMult = tWeapon["crit_mult"];
            rwd.damage = { tWeapon["damage_dcount"], tWeapon["damage_dsize"] };
    
            std::string weaponClass = tWeapon["weapon_class"];
            if ( stringContains(weaponClass, "Melee") )
            {
                rwd.weaponType = WeaponType::Melee;
            }
            else if ( stringContains(weaponClass, "Ranged") )
            {
                rwd.weaponType = WeaponType::Ranged;
            }
            else
            {
                AssertAlwaysMsg( fmt::format( "Unknown weapon class", weaponClass ) );
            }
            
            m_weaponData.push_back( std::move(rwd) );
        }
    
        auto vArmour = item["armour"];
        if ( vArmour != sol::nil )
        {
            sol::table const& tArmour = vArmour;
            ArmourData rad;
    
            rad.itemId = rit.id;
            rad.maxDex = tArmour.get_or( "max_dex", std::numeric_limits<int>::max() );
            rad.speed30 = tArmour.get_or( "max_speed", std::numeric_limits<int>::max() );
            rad.arcaneFailureChance = tArmour.get_or( "spell_failure", 0 );
            rad.armourBonus = tArmour.get_or( "armour_bonus", 0 );
            rad.armourCheck = tArmour.get_or( "armour_check", 0 );
            rad.shieldBonus = tArmour.get_or( "shield_bonus", 0 );
            rad.armourType = tArmour.get_or( "armour_type", std::string("") );
            
            m_armourData.push_back( std::move(rad) );
        }
        
        m_itemData.push_back( std::move(rit) );
    }
}

void ResourceDatabase::loadAllActionData()
{
    sol::table mods = m_lua.runLoadedScript( "data/Actions" );
    for ( auto const& [k, v] : mods )
    {
        sol::table const& data = v;
        ActionData robj;

        robj.name = data["name"];
        robj.id = k.as<sol::string_view>();

        std::string typeStr = data["type"];
        if (typeStr == "move")
        {
            robj.type = RawActionDataType::Move;
        }
        else if (typeStr == "attack")
        {
            robj.type = RawActionDataType::Attack;
        }
        else
        {
            AssertAlwaysMsg( fmt::format("Unknown action type: '{}'", typeStr) );
        }

        robj.speed = data["speed"];
        robj.provokes = data["provokes"];
        robj.description = data["description"];

        std::string spriteName = data["icon"];
        robj.sprite = SpritesheetKey( spriteName );

        m_actionData.push_back(robj);
    }
}

void ResourceDatabase::loadAllModifierData()
{
    sol::table mods = m_lua.runLoadedScript( "data/Modifiers" );

    for ( auto const& [k, v] : mods )
    {
        ModifierData robj;

        robj.id = k.as<sol::string_view>();
        robj.name = v.as<sol::table>()["name"];
        robj.effect = v.as<sol::table>()["effect"];
        robj.description = v.as<sol::table>()["desc"];

        m_modifierData.push_back(robj);
    }
}

void ResourceDatabase::loadAllChargenData()
{
    
    sol::table mods = m_lua.runLoadedScript( "data/Chargen" );
    
    for ( auto const& [k, v] : mods["classes"].get<sol::table>() )
    {
        PlayerData cgdata;
        sol::table const& data = v;
        
        cgdata.playerClass = data["name"];
        cgdata.sprite = data["sprite"].get<std::string>();
        cgdata.maxHP = data["max_hp"];
        cgdata.attrStr = data["attributes"][1];
        cgdata.attrDex = data["attributes"][2];
        cgdata.attrCon = data["attributes"][3];
        cgdata.attrInt = data["attributes"][4];
        cgdata.attrWis = data["attributes"][5];
        cgdata.attrCha = data["attributes"][6];
        cgdata.equippedItems =  data["starting_equipped_items"].get_or<std::vector<std::string>>( {} );
        cgdata.heldItems =  data["starting_held_items"].get_or<std::vector<std::string>>( {} );
        cgdata.featIds =  data["feats"].get_or<std::vector<std::string>>( {} );
        
        m_chargenData.push_back( std::move(cgdata) );
    }
        
    m_randomNames = mods["names"].get_or<std::vector<std::string>>( {} );
}

std::vector<std::string> const &ResourceDatabase::randomNames()
{
    return m_randomNames;
}



