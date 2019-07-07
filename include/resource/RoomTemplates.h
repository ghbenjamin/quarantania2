#pragma once

#include <string>
#include <utils/Containers.h>
#include <rapidjson/document.h>

struct RoomTemplate
{
};


class RoomTemplateManager
{
public:
    RoomTemplateManager() = default;
    ~RoomTemplateManager() = default;

   void loadAllTemplates( std::string const& path );

private:

    Vector2i parseKeyName( std::string const& name );

    std::unordered_map<Vector2i, std::vector<RoomTemplate>, Vector2Hash<int>> m_templates;

};