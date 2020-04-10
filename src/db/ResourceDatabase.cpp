#include <db/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <utils/Assert.h>
#include <utils/Json.h>

ResourceDatabase::ResourceDatabase()
{
    loadAllItemData();
    loadAllCreatureData();

}

ResourceDatabase::~ResourceDatabase()
{
}

RawItemData ResourceDatabase::itemFromName(std::string_view name)
{
    auto it = std::find_if( m_itemData.begin(), m_itemData.end(),
            [name](auto const& item){
        return item.name == name;
    });

    Assert( it != m_itemData.end() );
    return RawItemData( *it );
}

RawWeaponData ResourceDatabase::weaponFromName(std::string_view name)
{
    auto it = std::find_if( m_weaponData.begin(), m_weaponData.end(),
            [name](auto const& item){
                return item.item_name == name;
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

RawArmourData ResourceDatabase::armourFromName(std::string_view name)
{
    auto it = std::find_if( m_armourData.begin(), m_armourData.end(),
            [name](auto const& item){
                return item.item_name == name;
            });

    Assert( it != m_armourData.end() );
    return RawArmourData( *it );
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
        rcd.alignment = cr.FindMember( "alignment" )->value.GetString();
        rcd.creature_type = cr.FindMember( "creature_type" )->value.GetString();

        if ( cr.HasMember("description") )
        {
            rcd.description = cr.FindMember( "description" )->value.GetString();
        }

        if ( cr.HasMember("creature_subtype") )
        {
            rcd.creature_subtype = cr.FindMember( "creature_subtype" )->value.GetString();
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

        rcd.attr_str = cr.FindMember("strength")->value.GetInt();
        rcd.attr_dex = cr.FindMember("dexterity")->value.GetInt();
        rcd.attr_con = cr.FindMember("constitution")->value.GetInt();
        rcd.attr_int = cr.FindMember("intelligence")->value.GetInt();
        rcd.attr_wis = cr.FindMember("wisdom")->value.GetInt();
        rcd.attr_cha = cr.FindMember("charisma")->value.GetInt();

        rcd.save_fort = cr.FindMember("fortitude")->value.GetInt();
        rcd.save_ref = cr.FindMember("reflex")->value.GetInt();
        rcd.save_will = cr.FindMember("will")->value.GetInt();

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
        rit.item_type = it.FindMember( "item_type" )->value.GetString();
        rit.sprite = { it.FindMember( "sprite_sheet" )->value.GetString(),
                       it.FindMember( "sprite_name" )->value.GetString()};

        if ( it.HasMember( "aura_strength" ) )
        {
            rit.aura_strength = it.FindMember( "aura_strength" )->value.GetString();
        }

        if ( it.HasMember( "aura_value" ) )
        {
            rit.aura_value = it.FindMember( "aura_value" )->value.GetString();
        }

        if ( it.HasMember( "slot" ) )
        {
            rit.slot = it.FindMember( "slot" )->value.GetString();
        }

        if ( it.HasMember( "weapon" ) )
        {
            RawWeaponData rwd;
            auto wObj = it.FindMember( "weapon" )->value.GetObject();

            rwd.item_name = rit.name;
            rwd.crit_lower = wObj.FindMember( "crit_lower" )->value.GetInt();
            rwd.crit_mult = wObj.FindMember( "crit_mult" )->value.GetInt();
            rwd.damage = wObj.FindMember( "damage" )->value.GetString();
            rwd.damage_type = wObj.FindMember( "damage_type" )->value.GetString();
            rwd.proficiency = wObj.FindMember( "proficiency" )->value.GetString();
            rwd.weapon_class = wObj.FindMember( "weapon_class" )->value.GetString();

            if ( wObj.HasMember("Special") )
            {
                rwd.specials = wObj.FindMember( "Special" )->value.GetString();
            }

            m_weaponData.push_back( std::move(rwd) );
        }

        if ( it.HasMember( "armour" ) )
        {
            RawArmourData rad;
            auto aObj = it.FindMember("armour")->value.GetObject();

            rad.item_name = rit.name;

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
                rad.armour_type = aObj.FindMember( "Armor Type" )->value.GetString();
            }

            m_armourData.push_back( std::move(rad) );
        }


        m_itemData.push_back( std::move(rit) );
    }
}

