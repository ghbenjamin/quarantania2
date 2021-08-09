#include <game/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <utils/Json.h>
#include <utils/StringUtils.h>
#include <game/GameParse.h>

ResourceDatabase::ResourceDatabase()
{
    loadAllItemData();
    loadAllCreatureData();
    loadAllObjectData();
    loadAllActionData();
    loadAllFeatData();
    loadAllChargenData();
}

ItemData ResourceDatabase::itemFromName(std::string_view name)
{
    auto it = std::find_if( m_itemData.begin(), m_itemData.end(),
        [name](auto const& item){ return item.name == name; });

    AssertMsg( it != m_itemData.end(), fmt::format( "Unexpected item: {}", name ) );
    return ItemData( *it );
}

WeaponData ResourceDatabase::weaponFromName(std::string_view name)
{
    auto it = std::find_if( m_weaponData.begin(), m_weaponData.end(),
        [name](auto const& item){ return item.itemName == name; });

    Assert( it != m_weaponData.end() );
    return WeaponData( *it );
}

CreatureData ResourceDatabase::creatureFromName(std::string_view name)
{
    auto it = std::find_if( m_creatureData.begin(), m_creatureData.end(),
        [name](auto const& item){ return item.name == name; });

    AssertMsg( it != m_creatureData.end(), fmt::format("Unknown creature '{}'", name)  );
    return CreatureData( *it );
}

ArmourData ResourceDatabase::armourFromName(std::string_view name)
{
    auto it = std::find_if( m_armourData.begin(), m_armourData.end(),
        [name](auto const& item){ return item.itemName == name; });

    Assert( it != m_armourData.end() );
    return ArmourData( *it );
}

ObjectData ResourceDatabase::objectFromName( std::string_view name)
{
    auto it = std::find_if( m_objectData.begin(), m_objectData.end(),
        [name](auto const& item){ return item.name == name; });

    Assert( it != m_objectData.end() );
    return ObjectData( *it );
}

ActionData ResourceDatabase::actionFromId(std::string_view id)
{
    auto it = std::find_if( m_actionData.begin(), m_actionData.end(),
        [id](auto const& item){ return item.id == id; });

    Assert( it != m_actionData.end() );
    return ActionData( *it );
}

FeatData ResourceDatabase::featFromId( std::string_view id )
{
    auto it = std::find_if( m_featData.begin(), m_featData.end(),
        [id](auto const& item){ return item.id == id; });
    
    Assert( it != m_featData.end() );
    return FeatData( *it );
}

ChargenData ResourceDatabase::chargenFromClassId( std::string_view id )
{
    auto it = std::find_if( m_chargenData.begin(), m_chargenData.end(),
                            [id](auto const& item){ return item.id == id; });
    
    Assert( it != m_chargenData.end() );
    return *it;
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
        rcd.experience = cr["xp"];
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
        rcd.baseAttackBonus = cr["base_attack"];
        rcd.combatManeuverBonus = cr["cmb"];
        rcd.combatManeuverDefence = cr["cmd"];
        rcd.initiative = cr["initative"];
        rcd.speed = cr["speed"];

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
                rcd.feats.emplace_back( feat.value() );
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
    auto doc = utils::json::loadFromPath("../resource/data/items.json" );
    for ( auto const& it : doc )
    {
        ItemData rit;

        rit.name = it["name"];
        rit.weight = it["weight"];
        rit.value = it["price"];
        rit.description = it["description"];
        rit.itemType = it["item_type"];
        rit.sprite = { it["sprite_sheet"].get<std::string>(), it["sprite_name"].get<std::string>() };

        if ( it.contains( "aura_strength" ) )
        {
            rit.auraStrength = it["aura_strength"];
        }

        if ( it.contains( "aura_value" ) )
        {
            rit.auraValue = it["aura_value"];
        }

        if ( it.contains( "slot" ) )
        {
            rit.slot = it["slot"];
        }

        if ( it.contains( "weapon" ) )
        {
            WeaponData rwd;
            auto wObj = it["weapon"];

            rwd.itemName = rit.name;
            rwd.critLower = wObj["crit_lower"];
            rwd.critMult = wObj["crit_mult"];
            rwd.damage = {
                    wObj[ "damage_dcount"],
                    wObj[ "damage_dsize"],
            };

            rwd.damageType = wObj["damage_type"];

            std::string weaponClass = wObj["weapon_class"];
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

            if ( wObj.contains("Special") )
            {
                rwd.specials = wObj["Special"];
            }

            std::string profStr = wObj["proficiency"];
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

        if ( it.contains( "armour" ) )
        {
            ArmourData rad;
            auto aObj = it["armour"];

            rad.itemName = rit.name;

            if ( aObj.contains("Arcane Spell Failure Chance") )
            {
                rad.arcaneFailureChance = aObj[ "Arcane Spell Failure Chance" ];
            }

            if ( aObj.contains("Armour Bonus") )
            {
                rad.armourBonus = aObj[ "Armour Bonus" ];
            }

            if ( aObj.contains("Shield Bonus") )
            {
                rad.shieldBonus = aObj[ "Shield Bonus" ];
            }

            if ( aObj.contains("Armor Check Penalty") )
            {
                rad.armourCheck = aObj[ "Armor Check Penalty" ];
            }

            if ( aObj.contains("Maximum Dex Bonus") )
            {
                rad.maxDex = aObj[ "Maximum Dex Bonus" ];
            }
            else
            {
                rad.maxDex = std::numeric_limits<int>::max();
            }

            if ( aObj.contains("Speed 20") )
            {
                rad.speed20 = aObj[ "Speed 20" ];
            }

            if ( aObj.contains("Speed 30") )
            {
                rad.speed30 = aObj[ "Speed 30" ];
            }

            if ( aObj.contains("Armor Type") )
            {
                rad.armourType = aObj["Armor Type"];
            }

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
    auto doc = utils::json::loadFromPath("../resource/data/actions.json" );
    for ( auto const& it : doc )
    {
        ActionData robj;

        robj.name = it["name"];
        robj.id = it["id"];

        std::string typeStr = it["type"];
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

        robj.speed = EnumParse::actionSpeed( it["speed"] );
        robj.provokes = it["provokes"];
        robj.description = it["description"];
        robj.sprite = SpritesheetKey{ it["icon"] };

        m_actionData.push_back(robj);
    }
}

void ResourceDatabase::loadAllFeatData()
{
//    auto doc = utils::json::loadFromPath("../resource/data/feats.json" );
//    for ( auto const& it : doc )
//    {
//        FeatData robj;
//
//        robj.id = it["id"];
//        robj.name = it["name"];
//        robj.benefit = it["benefit"];
//        robj.description = it["desc"];
//
//        m_featData.push_back(robj);
//    }
}

void ResourceDatabase::loadAllChargenData()
{
    auto doc = utils::json::loadFromPath("../resource/data/starting_chars.json" );
    
    for ( auto const& classObj : doc["classes"] )
    {
        ChargenData cgdata;
        cgdata.id = classObj["id"];
        cgdata.name = classObj["name"];
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
            cgdata.feats.push_back( item );
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



