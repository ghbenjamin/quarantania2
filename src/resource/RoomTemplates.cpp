#include <resource/RoomTemplates.h>
#include <utils/Json.h>
#include <utils/Logging.h>
#include <rapidjson/document.h>

void RoomTemplateManager::loadAllTemplates(std::string const &path)
{
    rapidjson::Document doc = JsonUtils::loadFromPath( path );

    auto basicObj = doc.FindMember( "basic_rooms" )->value.GetObject();

    for ( auto const& obj_it : basicObj )
    {
        auto obj = obj_it.value.GetArray();
        for ( auto const& val_it : obj )
        {
            auto kn = parseKeyName(obj_it.name.GetString());

        }
    }
}


Vector2i RoomTemplateManager::parseKeyName(std::string const &name)
{
    return Vector2i();
}
