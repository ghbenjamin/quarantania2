#include <game/Enemy.h>
#include <utils/Json.h>
#include <utils/Logging.h>
#include <utils/Assert.h>
#include <db/ResourceDatabase.h>

Enemy::Enemy(EnemyData const &data)
: m_data(data)
{
}

SpritesheetKey const &Enemy::sprite() const
{
    return m_data.sprite;
}

EnemyData const &Enemy::data() const
{
    return m_data;
}

void EnemyManager::loadAllData()
{
// MSVC compiler error
#ifdef GetObject
#undef GetObject
#endif

    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/enemies.json" );
    auto rootObj = doc.GetObject();

    for ( auto const& obj : rootObj )
    {
        EnemyData data{};

        // If this entry in the map has a 'parent' listed, load the data of the parent from our existing entries
        // NOTE: this means that entries must be written in and consumed in order so that descendants come last
        if ( obj.value.HasMember("parent") )
        {
            auto parent = obj.value.FindMember("parent")->value.GetString();
            auto it = m_enemyData.find(parent);
            AssertMsg( it != m_enemyData.end(), "Parent of enemy object doesn't exist" );

            data = it->second;
        }

        auto entryName = obj.value.FindMember("name");
        if ( entryName != obj.value.MemberEnd() )
        {
            data.name = entryName->value.GetString();
        }

        auto entrySprite = obj.value.FindMember("sprite");
        if ( entrySprite != obj.value.MemberEnd() )
        {
            auto arr = entrySprite->value.GetArray();
            data.sprite = { arr[0].GetString(), arr[1].GetString() };
        }

        auto entryXP = obj.value.FindMember("xp");
        if ( entryXP != obj.value.MemberEnd() )
        {
            data.xp = entryXP->value.GetInt();
        }

        auto entry_maxHP = obj.value.FindMember("max_hp");
        if ( entry_maxHP != obj.value.MemberEnd() )
        {
            data.maxHP = entry_maxHP->value.GetInt();
        }

        auto entryAC = obj.value.FindMember("ac");
        if ( entryAC != obj.value.MemberEnd() )
        {
            data.ac = entryAC->value.GetInt();
        }

        auto entrySaves = obj.value.FindMember("saves");
        if ( entrySaves != obj.value.MemberEnd() )
        {
            auto arr = entrySaves->value.GetArray();
            data.fortSave = arr[0].GetInt();
            data.refSave = arr[1].GetInt();
            data.willSave = arr[2].GetInt();
        }

        auto entryAbilityScores = obj.value.FindMember("stat_block");
        if ( entryAbilityScores != obj.value.MemberEnd() )
        {
            auto arr = entryAbilityScores->value.GetArray();
            data.strScore = arr[0].GetInt();
            data.dexScore = arr[1].GetInt();
            data.conScore = arr[2].GetInt();
            data.intScore = arr[3].GetInt();
            data.wisScore = arr[4].GetInt();
            data.chaScore = arr[5].GetInt();
        }

        auto entrySpeed = obj.value.FindMember("speed");
        if ( entrySpeed != obj.value.MemberEnd() )
        {
            data.speed = entrySpeed->value.GetInt();
        }

        auto entryAttack = obj.value.FindMember("attack");
        if ( entryAttack != obj.value.MemberEnd() )
        {
            data.attack = DiceRoll::parseFromString( entryAttack->value.GetString() );
        }

        m_enemyData.emplace(obj.name.GetString(), std::move(data));
    }
}

Enemy EnemyManager::createEnemy(std::string const &name) const
{
    auto it = m_enemyData.find(name);
    AssertMsg( it != m_enemyData.end(), "Creation of unknown enemy" );

    return Enemy { it->second };
}
