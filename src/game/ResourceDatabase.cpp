#include <game/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <utils/Json.h>
#include <utils/StringUtils.h>
#include <game/GameParse.h>
#include "resource/ResourceManager.h"

ResourceDatabase::ResourceDatabase()
{
    loadAllItemData();
    loadAllCreatureData();
    loadAllObjectData();
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

ObjectData ResourceDatabase::objectFromName( std::string_view name)
{
    auto it = std::find_if( m_objectData.begin(), m_objectData.end(),
        [name](auto const& item){ return item.name == name; });

    Assert( it != m_objectData.end() );
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
    auto doc = utils::json::loadFromPath("../resource/data/creatures.json" );
    for ( auto const& cr : doc )
    {
        CreatureData rcd;

        rcd.name = cr["name"];
        rcd.alignment = EnumParse::alignment( cr["alignment"] );
        rcd.creatureType = cr["creature_type"];
        rcd.sprite = { cr["sprite"] };
        rcd.xp = cr["xp"];
        rcd.maxHP = cr["hp"];
        rcd.attrStr = cr["attr_str"];
        rcd.attrDex = cr["attr_dex"];
        rcd.attrCon = cr["attr_con"];
        rcd.attrInt = cr["attr_int"];
        rcd.attrWis = cr["attr_wis"];
        rcd.attrCha = cr["attr_cha"];
        rcd.saveFort = cr["save_fort"];
        rcd.saveRef = cr["save_ref"];
        rcd.saveWill = cr["save_will"];
        rcd.baseSpeed = cr["speed"];

        if ( cr.contains("description") )
        {
            rcd.description = cr["description"];
        }

        if ( cr.contains("creature_subtype") )
        {
            auto subtypeArr = cr["creature_subtype"];
            for ( auto const& stype : subtypeArr.items() )
            {
                rcd.creatureSubtypes.emplace_back( stype.value().get<std::string>() );
            }
        }

        if ( cr.contains("damage_resistance") )
        {
            for (auto const& dr : cr["damage_resistance"].items() )
            {
                std::string k = dr.key();
                int v = dr.value();
                rcd.damageResistance[k] = v;
            }
        }

        if ( cr.contains("elemental_resistance") )
        {
            for (auto const& dr : cr["elemental_resistance"].items() )
            {
                ElementalDamageType k = EnumParse::elementalDamageType( dr.key() );
                int v = dr.value();
                rcd.elementalResistance[k] = v;
            }
        }

        if ( cr.contains("feats") )
        {
            for ( auto const& feat : cr["feats"].items() )
            {
                rcd.featIds.emplace_back( feat.value() );
            }
        }

        if ( cr.contains("immune") )
        {
            for ( auto const& immune : cr["immune"].items() )
            {
                rcd.immune.emplace_back( immune.value() );
            }
        }

        if ( cr.contains("senses") )
        {
            for ( auto const& senses : cr["senses"].items() )
            {
                rcd.senses.emplace_back( senses.value() );
            }
        }

        if ( cr.contains("weaknesses") )
        {
            for ( auto const& weakness : cr["weaknesses"].items() )
            {
                rcd.weaknesses.emplace_back( EnumParse::elementalDamageType( weakness.value() ) );
            }
        }

        if ( cr.contains("melee") )
        {
            for ( auto const& melee : cr["melee"].items() )
            {
                auto meleeObj = melee.value();

                CreatureAttack creatureAttack;

                creatureAttack.name = meleeObj["name"];
                creatureAttack.count = 1;
                creatureAttack.toHit =  meleeObj["to_hit"];
                creatureAttack.stats  = parseDiceRoll( meleeObj["damage"] );

                if ( meleeObj.contains("count"))
                {
                    creatureAttack.count = meleeObj["count"];
                }

                rcd.attacks.push_back(creatureAttack);
            }
        }


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
            rwd.damageType = tWeapon["damage_type"];
    
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
    
            auto vSpecial = item["Special"];
            if ( vSpecial != sol::nil )
            {
                rwd.specials = vSpecial;
            }
    
            std::string profStr = tWeapon["proficiency"];
            if ( profStr == "Simple" )
            {
                rwd.proficiency = WeaponProficiency::Simple;
            }
            else if ( profStr == "Martial" )
            {
                rwd.proficiency = WeaponProficiency::Martial;
            }
            else
            {
                rwd.proficiency = WeaponProficiency::Exotic;
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


void ResourceDatabase::loadAllObjectData()
{
    auto doc = utils::json::loadFromPath("../resource/data/objects.json" );
    for ( auto const& it : doc )
    {
        ObjectData robj;

        robj.name = it["name"];
        robj.description = it["description"];
        robj.type = it["type"];

        for ( auto const& sprite_obj : it["sprites"] )
        {
            robj.sprites.emplace_back( sprite_obj["sprite_sheet"].get<std::string>(),
                    sprite_obj["sprite_name"].get<std::string>() );
        }

        m_objectData.push_back( robj );
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

        robj.speed = EnumParse::actionSpeed( data["speed"] );
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
    auto doc = utils::json::loadFromPath("../resource/data/starting_chars.json" );
    
    for ( auto const& classObj : doc["classes"] )
    {
        PlayerData cgdata;
        cgdata.playerClass = classObj["name"];
        
        cgdata.sprite = classObj["sprite"].get<std::string>();
        cgdata.maxHP = classObj["max_hp"];
        
        cgdata.attrStr = classObj["attributes"][0];
        cgdata.attrDex = classObj["attributes"][1];
        cgdata.attrCon = classObj["attributes"][2];
        cgdata.attrInt = classObj["attributes"][3];
        cgdata.attrWis = classObj["attributes"][4];
        cgdata.attrCha = classObj["attributes"][5];
        
        for ( auto const& item : classObj["starting_equipped_items"] )
        {
            cgdata.equippedItems.push_back( item );
        }
    
        for ( auto const& item : classObj["starting_held_items"] )
        {
            cgdata.heldItems.push_back( item );
        }
    
        for ( auto const& item : classObj["feats"] )
        {
            cgdata.featIds.push_back( item );
        }
        
        m_chargenData.push_back( std::move(cgdata) );
    }
    
    for ( auto const& name : doc["names"] )
    {
        m_randomNames.push_back(name);
    }
}

std::vector<std::string> const &ResourceDatabase::randomNames()
{
    return m_randomNames;
}



