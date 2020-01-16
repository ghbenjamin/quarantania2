#include <game/Weapon.h>
#include <utils/Json.h>
#include <utils/Logging.h>
#include <utils/Assert.h>

void WeaponManager::loadAllData()
{
// MSVC compiler error
#ifdef GetObject
#undef GetObject
#endif

    rapidjson::Document doc = JsonUtils::loadFromPath( "../resource/data/weapons.json" );
    auto rootObj = doc.GetObject();
    auto meleeArray = rootObj.FindMember("melee")->value.GetArray();

    for ( auto const& arrItem : meleeArray )
    {
        auto obj = arrItem.GetObject();

        WeaponData data{};
        std::string buf;

        auto entryName = obj.FindMember("Name");
        if ( entryName != obj.MemberEnd() )
        {
            data.name = entryName->value.GetString();
        }

        auto entryHands = obj.FindMember("Hands");
        if ( entryHands != obj.MemberEnd() )
        {
            buf = entryHands->value.GetString();
            if ( buf == "1H" )
            {
                data.handedness = WeaponHandedness::OneHanded;
            }
            else if ( buf == "2H" )
            {
                data.handedness = WeaponHandedness::TwoHanded;
            }
            else
            {
                AssertAlways();
            }
        }

        auto entryType = obj.FindMember("Type");
        if ( entryType != obj.MemberEnd() )
        {
            buf = entryType->value.GetString();
            if ( buf == "Simple" )
            {
                data.type = WeaponType::Simple;
            }
            else if ( buf == "Martial" )
            {
                data.type = WeaponType::Martial;
            }
            else if ( buf == "Exotic" )
            {
                data.type = WeaponType::Exotic;
            }
            else
            {
                AssertAlways();
            }
        }

        auto entryDmg = obj.FindMember("Damage");
        if ( entryDmg != obj.MemberEnd() )
        {
            buf = entryDmg->value.GetString();
            data.damage = DiceRoll::parseFromString(buf);
        }

        auto entryCrit = obj.FindMember("Crit");
        if ( entryCrit != obj.MemberEnd() )
        {
            buf = entryCrit->value.GetString();
            data.criticals = parseCritLine(buf);
        }

        auto entryReach = obj.FindMember("Reach");
        if ( entryReach != obj.MemberEnd() )
        {
            data.reach = std::atoi( entryReach->value.GetString() );
        }

        auto entryWeight = obj.FindMember("Weight");
        if ( entryWeight != obj.MemberEnd() )
        {
            data.weight = std::atoi( entryWeight->value.GetString() );
        }

        // TODO Damage type?

        m_weaponData.emplace( data.name, std::move(data) );
    }
}

CritData WeaponManager::parseCritLine(std::string const& data)
{
    CritData crit{};
    std::smatch match;

    if ( std::regex_search( data, match, CritRegex ) && match.size() > 1 )
    {
        if ( !match.str(2).empty() )
        {
            crit.minCritRange = std::atoi( match.str(2).c_str() );
        }
        if ( !match.str(3).empty() )
        {
            crit.maxCritRange = std::atoi( match.str(3).c_str() );
        }

        crit.critMultiplier = std::atoi( match.str(4).c_str() );
    }
    else
    {
        // Bad crit string
        AssertAlways();
    }

    return crit;
}

WeaponData const& WeaponManager::getWeaponData(std::string const &name)
{
    auto it = m_weaponData.find( name );
    AssertMsg( it != m_weaponData.end(), fmt::format( "Unknown weapon \"{}\"", name ).c_str() );
    return it->second;
}
