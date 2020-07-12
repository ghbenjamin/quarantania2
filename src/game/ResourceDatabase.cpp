#include <game/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <utils/Assert.h>
#include <utils/Json.h>
#include <utils/Random.h>
#include <game/Items.h>

ResourceDatabase::ResourceDatabase()
{
    loadAllItemData();
    loadAllCreatureData();
    loadAllPlayerData();
    loadAllObjectData();
    loadAllRoomTemplateData();
}

RawItemData ResourceDatabase::itemFromName(std::string_view name)
{
    auto it = std::find_if( m_itemData.begin(), m_itemData.end(),
            [name](auto const& item){
        return item.name == name;
    });

    AssertMsg( it != m_itemData.end(), fmt::format( "Unexpected item: {}", name ) );
    return RawItemData( *it );
}

RawWeaponData ResourceDatabase::weaponFromName(std::string_view name)
{
    auto it = std::find_if( m_weaponData.begin(), m_weaponData.end(),
            [name](auto const& item){
                return item.itemName == name;
            });

    Assert( it != m_weaponData.end() );
    return RawWeaponData( *it );
}

RawCreatureData ResourceDatabase::creatureFromName(std::string_view name)
{
    auto it = std::find_if( m_creatureData.begin(), m_creatureData.end(),
            [name](auto const& item){
                return item.name == name;
            });

    Assert( it != m_creatureData.end() );
    return RawCreatureData( *it );
}

RawPlayerRaceData ResourceDatabase::playerRaceFromName(std::string_view name)
{
    auto it = std::find_if( m_playerRaceData.begin(), m_playerRaceData.end(),
            [name](auto const& item){
                return item.name == name;
            });

    Assert( it != m_playerRaceData.end() );
    return RawPlayerRaceData( *it );
}

RawPlayerClassData ResourceDatabase::playerClassFromName(std::string_view name)
{
    auto it = std::find_if( m_playerClassData.begin(), m_playerClassData.end(),
            [name](auto const& item){
                return item.name == name;
            });

    Assert( it != m_playerClassData.end() );
    return RawPlayerClassData( *it );
}

RawArmourData ResourceDatabase::armourFromName(std::string_view name)
{
    auto it = std::find_if( m_armourData.begin(), m_armourData.end(),
            [name](auto const& item){
                return item.itemName == name;
            });

    Assert( it != m_armourData.end() );
    return RawArmourData( *it );
}

RawObjectData ResourceDatabase::objectFromName(std::string_view name)
{
    auto it = std::find_if( m_objectData.begin(), m_objectData.end(),
            [name](auto const& item){
                return item.name == name;
            });

    Assert( it != m_objectData.end() );
    return RawObjectData( *it );
}


void ResourceDatabase::loadAllCreatureData()
{
    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/creatures.json" );
    for ( auto const& cr_raw : doc.GetArray() )
    {
        RawCreatureData rcd;
        auto cr = cr_raw.GetObject();

        rcd.name = cr.FindMember( "name" )->value.GetString();
        rcd.xp = cr.FindMember( "xp" )->value.GetInt();
        rcd.hp = cr.FindMember( "hp" )->value.GetInt();
        rcd.alignment = cr.FindMember( "alignment" )->value.GetString();
        rcd.creatureType = cr.FindMember("creature_type" )->value.GetString();

        rcd.sprite = { cr.FindMember( "sprite_sheet" )->value.GetString(),
                       cr.FindMember( "sprite_name" )->value.GetString()};

        if ( cr.HasMember("description") )
        {
            rcd.description = cr.FindMember( "description" )->value.GetString();
        }

        if ( cr.HasMember("creature_subtype") )
        {
            rcd.creatureSubtype = cr.FindMember("creature_subtype" )->value.GetString();
        }

        if ( cr.HasMember("dr") )
        {
            rcd.damageResist = cr.FindMember( "dr" )->value.GetString();
        }

        if ( cr.HasMember("feats") )
        {
            rcd.feats = cr.FindMember( "feats" )->value.GetString();
        }

        if ( cr.HasMember("immune") )
        {
            rcd.immune = cr.FindMember( "immune" )->value.GetString();
        }

        if ( cr.HasMember("languages") )
        {
            rcd.languages = cr.FindMember( "languages" )->value.GetString();
        }

        if ( cr.HasMember("resist") )
        {
            rcd.resist = cr.FindMember( "resist" )->value.GetString();
        }
        if ( cr.HasMember("senses") )
        {
            rcd.senses = cr.FindMember( "senses" )->value.GetString();
        }

        rcd.attrStr = cr.FindMember("strength")->value.GetInt();
        rcd.attrDex = cr.FindMember("dexterity")->value.GetInt();
        rcd.attrCon = cr.FindMember("constitution")->value.GetInt();
        rcd.attrInt = cr.FindMember("intelligence")->value.GetInt();
        rcd.attrWis = cr.FindMember("wisdom")->value.GetInt();
        rcd.attrCha = cr.FindMember("charisma")->value.GetInt();

        rcd.saveFort = cr.FindMember("fortitude")->value.GetInt();
        rcd.saveRef = cr.FindMember("reflex")->value.GetInt();
        rcd.saveWill = cr.FindMember("will")->value.GetInt();

        rcd.bab = cr.FindMember("base_attack")->value.GetInt();
        rcd.cmb = cr.FindMember("cmb")->value.GetInt();
        rcd.cmd = cr.FindMember("cmd")->value.GetInt();

        m_creatureData.push_back( std::move(rcd) );
    }
}

void ResourceDatabase::loadAllItemData()
{
    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/items.json" );
    for ( auto const& it_raw : doc.GetArray() )
    {
        RawItemData rit;
        auto it = it_raw.GetObject();

        rit.name = it.FindMember( "name" )->value.GetString();
        rit.weight = it.FindMember( "weight" )->value.GetInt();
        rit.value = it.FindMember( "price" )->value.GetInt();
        rit.description = it.FindMember( "description" )->value.GetString();
        rit.itemType = it.FindMember("item_type" )->value.GetString();
        rit.sprite = { it.FindMember( "sprite_sheet" )->value.GetString(),
                       it.FindMember( "sprite_name" )->value.GetString()};

        if ( it.HasMember( "aura_strength" ) )
        {
            rit.auraStrength = it.FindMember("aura_strength" )->value.GetString();
        }

        if ( it.HasMember( "aura_value" ) )
        {
            rit.auraValue = it.FindMember("aura_value" )->value.GetString();
        }

        if ( it.HasMember( "slot" ) )
        {
            rit.slot = it.FindMember( "slot" )->value.GetString();
        }

        if ( it.HasMember( "weapon" ) )
        {
            RawWeaponData rwd;
            auto wObj = it.FindMember( "weapon" )->value.GetObject();

            rwd.itemName = rit.name;
            rwd.critLower = wObj.FindMember("crit_lower" )->value.GetInt();
            rwd.critMult = wObj.FindMember("crit_mult" )->value.GetInt();
            rwd.damage = {
                    wObj.FindMember( "damage_dcount" )->value.GetInt(),
                    wObj.FindMember( "damage_dsize" )->value.GetInt(),
            };

            rwd.damageType = wObj.FindMember("damage_type" )->value.GetString();
            rwd.weaponClass = wObj.FindMember("weapon_class" )->value.GetString();

            if ( wObj.HasMember("Special") )
            {
                rwd.specials = wObj.FindMember( "Special" )->value.GetString();
            }

            std::string profStr = wObj.FindMember( "proficiency" )->value.GetString();
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

        if ( it.HasMember( "armour" ) )
        {
            RawArmourData rad;
            auto aObj = it.FindMember("armour")->value.GetObject();

            rad.itemName = rit.name;

            if ( aObj.HasMember("Arcane Spell Failure Chance") )
            {
                rad.arcaneFailureChance = aObj.FindMember( "Arcane Spell Failure Chance" )->value.GetInt();
            }

            if ( aObj.HasMember("Armour Bonus") )
            {
                rad.armourBonus = aObj.FindMember( "Armour Bonus" )->value.GetInt();
            }

            if ( aObj.HasMember("Shield Bonus") )
            {
                rad.shieldBonus = aObj.FindMember( "Shield Bonus" )->value.GetInt();
            }

            if ( aObj.HasMember("Armor Check Penalty") )
            {
                rad.armourCheck = aObj.FindMember( "Armor Check Penalty" )->value.GetInt();
            }

            if ( aObj.HasMember("Maximum Dex Bonus") )
            {
                rad.maxDex = aObj.FindMember( "Maximum Dex Bonus" )->value.GetInt();
            }

            if ( aObj.HasMember("Speed 20") )
            {
                rad.speed20 = aObj.FindMember( "Speed 20" )->value.GetInt();
            }

            if ( aObj.HasMember("Speed 30") )
            {
                rad.speed30 = aObj.FindMember( "Speed 30" )->value.GetInt();
            }

            if ( aObj.HasMember("Armor Type") )
            {
                rad.armourType = aObj.FindMember("Armor Type" )->value.GetString();
            }

            m_armourData.push_back( std::move(rad) );
        }


        m_itemData.push_back( std::move(rit) );
    }
}

void ResourceDatabase::loadAllPlayerData()
{
    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/player.json" );

    auto classesArr = doc.FindMember( "classes" )->value.GetArray();
    for ( auto const& item : classesArr )
    {
        RawPlayerClassData pcd;
        auto itemObj = item.GetObject();

        pcd.name = itemObj.FindMember("name")->value.GetString();
        pcd.description = itemObj.FindMember("description")->value.GetString();
        pcd.hitDie = itemObj.FindMember( "hit_die" )->value.GetInt();
        pcd.skillsPerLevel = itemObj.FindMember( "skills_per_level" )->value.GetInt();

        auto babArr = itemObj.FindMember( "bab_by_level" )->value.GetArray();
        auto fortArr = itemObj.FindMember( "fort_by_level" )->value.GetArray();
        auto refArr = itemObj.FindMember( "ref_by_level" )->value.GetArray();
        auto willArr = itemObj.FindMember( "will_by_level" )->value.GetArray();

        for ( int i = 0; i < 20; i++ )
        {
            pcd.babByLevel[i] = babArr[i].GetInt();
            pcd.fortByLevel[i] = fortArr[i].GetInt();
            pcd.refByLevel[i] = refArr[i].GetInt();
            pcd.willByLevel[i] = willArr[i].GetInt();
        }

        m_playerClassData.push_back( pcd );
    }

    auto racesArr = doc.FindMember( "races" )->value.GetArray();
    for ( auto const& item : racesArr )
    {
        RawPlayerRaceData prd;
        auto itemObj = item.GetObject();

        prd.name = itemObj.FindMember("name")->value.GetString();
        prd.baseSpeed = itemObj.FindMember("base_speed")->value.GetInt();

        m_playerRaceData.push_back(prd);
    }
}

Alignment ResourceDatabase::parseAlignmentFromStr(std::string_view sv)
{
    if ( sv == "LG" )
    {
        return Alignment::LG;
    }
    else if ( sv == "NG" )
    {
        return Alignment::NG;
    }
    else if ( sv == "CG" )
    {
        return Alignment::CG;
    }
    else if ( sv == "LN" )
    {
        return Alignment::LN;
    }
    else if ( sv == "N" )
    {
        return Alignment::TN;
    }
    else if ( sv == "CN" )
    {
        return Alignment::CN;
    }
    else if ( sv == "LE" )
    {
        return Alignment::LE;
    }
    else if ( sv == "NE" )
    {
        return Alignment::NE;
    }
    else if ( sv == "CE" )
    {
        return Alignment::CE;
    }
    else
    {
        AssertAlwaysMsg( fmt::format( "Unexpected alignment: '{}'", sv ) );
        return Alignment::TN;
    }
}

EquipSlot ResourceDatabase::parseEquipSlotFromStr(std::string_view sv)
{
    if ( sv == "armor" )
    {
        return EquipSlot::Armor;
    }
    else if ( sv == "arms" )
    {
        return EquipSlot::Arms;
    }
    else if ( sv == "belt" )
    {
        return EquipSlot::Belt;
    }
    else if ( sv == "body" )
    {
        return EquipSlot::Body;
    }
    else if ( sv == "chest" )
    {
        return EquipSlot::Chest;
    }
    else if ( sv == "eyes" )
    {
        return EquipSlot::Eyes;
    }
    else if ( sv == "feet" )
    {
        return EquipSlot::Feet;
    }
    else if ( sv == "hands" )
    {
        return EquipSlot::Hands;
    }
    else if ( sv == "head" )
    {
        return EquipSlot::Head;
    }
    else if ( sv == "headband" )
    {
        return EquipSlot::Headband;
    }
    else if ( sv == "neck" )
    {
        return EquipSlot::Neck;
    }
    else if ( sv == "ring" )
    {
        return EquipSlot::Ring;
    }
    else if ( sv == "shield" )
    {
        return EquipSlot::Shield;
    }
    else if ( sv == "shoulders" )
    {
        return EquipSlot::Shoulders;
    }
    else if ( sv == "wrists" )
    {
        return EquipSlot::Wrists;
    }
    else if ( sv == "weapon" )
    {
        return EquipSlot::Weapon;
    }
    else
    {
        return EquipSlot::None;
    }
}

ArmourType ResourceDatabase::parseArmourTypeFromStr(std::string_view sv)
{
    if ( sv == "Light armor" )
    {
        return ArmourType::Light;
    }
    else if ( sv == "Medium armor" )
    {
        return ArmourType::Medium;
    }
    else if ( sv == "Heavy armor" )
    {
        return ArmourType::Heavy;
    }
    if ( sv == "Shields" )
    {
        return ArmourType::Shield;
    }
    else
    {
        AssertAlwaysMsg( "Unknown armour type" );
        return ArmourType::Light;
    }
}

void ResourceDatabase::loadAllObjectData()
{
    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/objects.json" );
    for ( auto const& it_raw : doc.GetArray() )
    {
        RawObjectData robj;
        auto it = it_raw.GetObject();

        robj.name = it.FindMember("name")->value.GetString();
        robj.description = it.FindMember("description")->value.GetString();
        robj.type = it.FindMember("type")->value.GetString();

        for ( auto const& sprite_node : it.FindMember("sprites")->value.GetArray() )
        {
            auto sprite_obj = sprite_node.GetObject();
            robj.sprites.emplace_back( sprite_obj.FindMember("sprite_sheet")->value.GetString(),
                    sprite_obj.FindMember("sprite_name")->value.GetString() );
        }

        m_objectData.push_back( robj );
    }
}

void ResourceDatabase::loadAllRoomTemplateData()
{
    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/rooms.json" );

    for ( auto const& hnode : doc.FindMember("normal_rooms")->value.GetObject() )
    {
        int hval = std::atoi( hnode.name.GetString() );
        for ( auto const& vnode : hnode.value.GetObject() )
        {
            int vval = std::atoi( vnode.name.GetString() );
            Vector2i key = {hval, vval};

            for ( auto const& rt : vnode.value.GetArray() )
            {
                auto rt_obj = rt.GetObject();
                RawRoomTemplateData rawrt;
                rawrt.size = key;

                for ( auto const& objs_item : rt_obj.FindMember("objects")->value.GetArray() )
                {
                    RawRoomObjectData robjd;
                    auto objs_obj = objs_item.GetObject();

                    robjd.name = objs_obj.FindMember("name")->value.GetString();

                    auto offset_arr = objs_obj.FindMember("position")->value.GetArray();
                    robjd.offset = { offset_arr[0].GetInt(), offset_arr[1].GetInt() };

                    rawrt.objects.push_back( robjd );
                }

                m_roomTemplateData.emplace( key, rawrt );
            }
        }
    }

}

RawRoomTemplateData ResourceDatabase::randomRoomTemplate(Vector2i size, RandomInterface& ri)
{
    auto it = m_roomTemplateData.equal_range(size);

    if ( it.first == it.second )
    {
        // Default to empty room;
        return RawRoomTemplateData{size, {}};
    }

    auto rit = ri.randomElement( it.first, it.second );
    return rit->second;
}

