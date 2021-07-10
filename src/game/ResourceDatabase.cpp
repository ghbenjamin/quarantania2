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
    loadAllPlayerData();
    loadAllObjectData();
    loadAllActionData();
    loadAllFeatData();
}

ItemData ResourceDatabase::itemFromName(std::string_view name)
{
    auto it = std::find_if( m_itemData.begin(), m_itemData.end(),
            [name](auto const& item){
        return item.name == name;
    });

    AssertMsg( it != m_itemData.end(), fmt::format( "Unexpected item: {}", name ) );
    return ItemData( *it );
}

WeaponData ResourceDatabase::weaponFromName(std::string_view name)
{
    auto it = std::find_if( m_weaponData.begin(), m_weaponData.end(),
            [name](auto const& item){
                return item.itemName == name;
            });

    Assert( it != m_weaponData.end() );
    return WeaponData( *it );
}

CreatureData ResourceDatabase::creatureFromName(std::string_view name)
{
    auto it = std::find_if( m_creatureData.begin(), m_creatureData.end(),
            [name](auto const& item){
                return item.name == name;
            });

    AssertMsg( it != m_creatureData.end(), fmt::format("Unknown creature '{}'", name)  );
    return CreatureData( *it );
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

ArmourData ResourceDatabase::armourFromName(std::string_view name)
{
    auto it = std::find_if( m_armourData.begin(), m_armourData.end(),
            [name](auto const& item){
                return item.itemName == name;
            });

    Assert( it != m_armourData.end() );
    return ArmourData( *it );
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

ActionData ResourceDatabase::actionFromId(std::string_view id)
{
    auto it = std::find_if( m_actionData.begin(), m_actionData.end(),
            [id](auto const& item){
                return item.id == id;
            });

    Assert( it != m_actionData.end() );
    return ActionData( *it );
}

FeatData ResourceDatabase::featFromId( std::string_view id )
{
    auto it = std::find_if( m_featData.begin(), m_featData.end(),
                            [id](auto const& item){
                                return item.id == id;
                            });
    
    Assert( it != m_featData.end() );
    return FeatData( *it );
}


void ResourceDatabase::loadAllCreatureData()
{
    auto doc = Utils::Json::loadFromPath( "../resource/data/creatures.json" );
    for ( auto const& cr_raw : doc.GetArray() )
    {
        CreatureData rcd;
        auto cr = cr_raw.GetObject();

        rcd.name = cr.FindMember( "name" )->value.GetString();
        rcd.alignment = EnumParse::alignment(cr.FindMember("alignment")->value.GetString());
        rcd.creatureType = cr.FindMember("creature_type" )->value.GetString();
        rcd.sprite = { cr.FindMember( "sprite" )->value.GetString() };

        rcd.experience = cr.FindMember( "xp" )->value.GetInt();
        rcd.maxHP = cr.FindMember( "hp" )->value.GetInt();
        rcd.attrStr = cr.FindMember("attr_str")->value.GetInt();
        rcd.attrDex = cr.FindMember("attr_dex")->value.GetInt();
        rcd.attrCon = cr.FindMember("attr_con")->value.GetInt();
        rcd.attrInt = cr.FindMember("attr_int")->value.GetInt();
        rcd.attrWis = cr.FindMember("attr_wis")->value.GetInt();
        rcd.attrCha = cr.FindMember("attr_cha")->value.GetInt();
        rcd.saveFort = cr.FindMember("save_fort")->value.GetInt();
        rcd.saveRef = cr.FindMember("save_ref")->value.GetInt();
        rcd.saveWill = cr.FindMember("save_will")->value.GetInt();
        rcd.baseAttackBonus = cr.FindMember("base_attack")->value.GetInt();
        rcd.combatManeuverBonus = cr.FindMember("cmb")->value.GetInt();
        rcd.combatManeuverDefence = cr.FindMember("cmd")->value.GetInt();
        rcd.initiative = cr.FindMember("initative")->value.GetInt();
        rcd.speed = cr.FindMember("speed")->value.GetInt();

        if ( cr.HasMember("description") )
        {
            rcd.description = cr.FindMember( "description" )->value.GetString();
        }

        if ( cr.HasMember("creature_subtype") )
        {
            auto subtypeArr = cr.FindMember("creature_subtype" )->value.GetArray();
            for ( auto const& stype : subtypeArr )
            {
                rcd.creatureSubtypes.emplace_back( stype.GetString() );
            }
        }

        if ( cr.HasMember("damage_resistance") )
        {
            for (auto const& dr : cr.FindMember("damage_resistance")->value.GetObject() )
            {
                std::string k = dr.name.GetString();
                int v = dr.value.GetInt();
                rcd.damageResistance[k] = v;
            }
        }

        if ( cr.HasMember("elemental_resistance") )
        {
            for (auto const& dr : cr.FindMember("elemental_resistance")->value.GetObject() )
            {
                ElementalDamageType k = EnumParse::elementalDamageType( dr.name.GetString() );
                int v = dr.value.GetInt();
                rcd.elementalResistance[k] = v;
            }
        }

        if ( cr.HasMember("feats") )
        {
            auto featsArr = cr.FindMember("feats" )->value.GetArray();
            for ( auto const& feat : featsArr )
            {
                rcd.feats.emplace_back( feat.GetString() );
            }
        }

        if ( cr.HasMember("immune") )
        {
            auto immuneArr = cr.FindMember( "immune" )->value.GetArray();
            for ( auto const& immune : immuneArr )
            {
                rcd.immune.emplace_back( immune.GetString() );
            }
        }

        if ( cr.HasMember("senses") )
        {
            auto sensesArr = cr.FindMember( "senses" )->value.GetArray();
            for ( auto const& senses : sensesArr )
            {
                rcd.senses.emplace_back( senses.GetString() );
            }
        }

        if ( cr.HasMember("weaknesses") )
        {
            auto weaknessArr = cr.FindMember( "weaknesses" )->value.GetArray();
            for ( auto const& weakness : weaknessArr )
            {
                rcd.weaknesses.emplace_back( EnumParse::elementalDamageType( weakness.GetString() ) );
            }
        }

        if ( cr.HasMember("melee") )
        {
            auto meleeArr = cr.FindMember( "melee" )->value.GetArray();
            for ( auto const& melee :meleeArr )
            {
                auto meleeObj = melee.GetObject();
                CreatureAttack creatureAttack;

                creatureAttack.name = meleeObj.FindMember("name")->value.GetString();
                creatureAttack.count = 1;
                creatureAttack.toHit =  meleeObj.FindMember("to_hit")->value.GetInt();
                creatureAttack.stats  = parseDiceRoll( meleeObj.FindMember("damage")->value.GetString() );

                if ( meleeObj.HasMember("count"))
                {
                    creatureAttack.count = meleeObj.FindMember("count")->value.GetInt();
                }

                rcd.attacks.push_back(creatureAttack);
            }
        }


        m_creatureData.push_back( std::move(rcd) );
    }
}

void ResourceDatabase::loadAllItemData()
{
    auto doc = Utils::Json::loadFromPath( "../resource/data/items.json" );
    for ( auto const& it_raw : doc.GetArray() )
    {
        ItemData rit;
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
            WeaponData rwd;
            auto wObj = it.FindMember( "weapon" )->value.GetObject();

            rwd.itemName = rit.name;
            rwd.critLower = wObj.FindMember("crit_lower" )->value.GetInt();
            rwd.critMult = wObj.FindMember("crit_mult" )->value.GetInt();
            rwd.damage = {
                    wObj.FindMember( "damage_dcount" )->value.GetInt(),
                    wObj.FindMember( "damage_dsize" )->value.GetInt(),
            };

            rwd.damageType = wObj.FindMember("damage_type" )->value.GetString();

            std::string weaponClass = wObj.FindMember("weapon_class" )->value.GetString();
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
            ArmourData rad;
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
            else
            {
                rad.maxDex = std::numeric_limits<int>::max();
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
    auto doc = Utils::Json::loadFromPath( "../resource/data/player.json" );

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



void ResourceDatabase::loadAllObjectData()
{
    auto doc = Utils::Json::loadFromPath( "../resource/data/objects.json" );
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

void ResourceDatabase::loadAllActionData()
{
    auto doc = Utils::Json::loadFromPath( "../resource/data/actions.json" );
    for ( auto const& it_raw : doc.GetArray() )
    {
        ActionData robj;
        auto it = it_raw.GetObject();

        robj.name = it.FindMember("name")->value.GetString();
        robj.id = it.FindMember("id")->value.GetString();

        std::string typeStr = it.FindMember("type")->value.GetString();
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

        robj.speed = EnumParse::actionSpeed( it.FindMember("speed")->value.GetString() );
        robj.provokes = it.FindMember("provokes")->value.GetBool();
        robj.description = it.FindMember("description")->value.GetString();
        robj.sprite = SpritesheetKey{ it.FindMember("icon")->value.GetString() };

        m_actionData.push_back(robj);
    }
}

void ResourceDatabase::loadAllFeatData()
{
    auto doc = Utils::Json::loadFromPath( "../resource/data/feats.json" );
    for ( auto const& it_raw : doc.GetArray() )
    {
        FeatData robj;
        auto it = it_raw.GetObject();
    
        robj.id = it.FindMember("id")->value.GetString();
        robj.name = it.FindMember("name")->value.GetString();
        robj.benefit = it.FindMember("benefit")->value.GetString();
        robj.description = it.FindMember("desc")->value.GetString();
        
        m_featData.push_back(robj);
    }
}

