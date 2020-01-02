#include <resource/RoomTemplates.h>

#include <regex>

#include <utils/Json.h>
#include <utils/Logging.h>
#include <utils/Random.h>
#include <rapidjson/document.h>

void RoomTemplateManager::loadAllTemplates(std::string const &path)
{
    rapidjson::Document doc = JsonUtils::loadFromPath( path );

// MSVC compiler error
#ifdef GetObject
#undef GetObject
#endif

    // Basic rooms:

    auto basicObj = doc.FindMember( "basic_rooms" )->value.GetObject();

    // Walk over each room size
    for ( auto const& obj_it : basicObj )
    {
        auto obj = obj_it.value.GetArray();
        auto kn = parseKeyName( obj_it.name.GetString() );
        if (kn == Vector2i{0, 0})
        {
            continue;
        }

        // Walk over individual rooms
        for ( auto const& val_it : obj )
        {
            RoomTemplate roomTemplate;
            auto room_obj = val_it.GetObject();

            if ( room_obj.HasMember( "prefabs" ))
            {
                auto prefab_arr = room_obj.FindMember("prefabs")->value.GetArray();
                for ( auto const& prefab_it : prefab_arr )
                {
                    RoomTemplatePrefab rtp;
                    auto prefab_obj = prefab_it.GetObject();
                    std::string prefabName = prefab_obj.FindMember("name")->value.GetString();
                    rtp.type = prefabTypeFromName(prefabName);
                    auto pos_arr = prefab_obj.FindMember("position")->value.GetArray();
                    rtp.position = {
                        pos_arr[0].GetInt(),
                        pos_arr[1].GetInt()
                    };

                    roomTemplate.prefabs.push_back( std::move(rtp) );
                }
            }


            m_basicTemplates.emplace(kn, roomTemplate );
        }
    }

    // Special rooms
    // TODO Special rooms
}


Vector2i RoomTemplateManager::parseKeyName(std::string const &name)
{
    std::smatch match;
    Vector2i out;

    if ( std::regex_search( name, match, KEY_PARSE_REGEX) && match.size() > 1 )
    {
        int lhs = std::atoi( match.str(1).c_str() );
        int rhs = std::atoi( match.str(2).c_str() );

        out = {lhs, rhs};
    }
    else
    {
        out = Vector2i{0, 0};
    }

    return out;
}

RoomTemplate* RoomTemplateManager::randomTemplate(Vector2i roomSize, RandomGenerator& rd)
{
    auto it = m_basicTemplates.equal_range(roomSize);

    if ( it.first == it.second )
    {
        return nullptr;
    }

    auto rit = randomElement( it.first, it.second, rd );
    return &rit->second;
}
