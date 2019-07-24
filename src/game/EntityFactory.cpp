#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <rapidjson/document.h>
#include <utils/Json.h>

EntityFactory::EntityFactory(Level *parent)
: m_parent(parent)
{
}


std::unique_ptr<Player> EntityFactory::createPlayer(ImPlayerData &data, Vector2i startPos) const
{
    auto eref = m_parent->createEntity();
    auto sprite = ResourceManager::get().getSprite("kenney-chars", "example-char-1");

    m_parent->addComponent<Components::TilePosition>(eref, startPos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref);

    return std::make_unique<Player>( std::move(data), eref );
}

EntityRef EntityFactory::debugHighlight(Vector2i pos, std::string const& tile) const
{
    auto eref = m_parent->createEntity();
    auto sprite = ResourceManager::get().getSprite("kenney-tiles", tile);

    m_parent->addComponent<Components::TilePosition>(eref, pos);
    m_parent->addComponent<Components::Render>(eref, sprite);

    return eref;
}

EntityRef EntityFactory::createDoor(Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto sprite = ResourceManager::get().getSprite("kenney-tiles", "door-1");

    m_parent->addComponent<Components::TilePosition>(eref, pos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref);

    return eref;
}

EntityRef EntityFactory::createEntrance(Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto sprite = ResourceManager::get().getSprite("kenney-tiles", "grey-stairs-up");

    m_parent->addComponent<Components::TilePosition>(eref, pos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref);

    return eref;
}

EntityRef EntityFactory::createExit(Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto sprite = ResourceManager::get().getSprite("kenney-tiles", "grey-stairs-down");

    m_parent->addComponent<Components::TilePosition>(eref, pos);
    m_parent->addComponent<Components::Render>(eref, sprite);

    return eref;
}

void EntityFactory::loadAllPrefabs(std::string const &path)
{
    rapidjson::Document doc = JsonUtils::loadFromPath( path );

    auto static_objs = doc.FindMember( "static_objects" )->value.GetArray();

    for ( auto const& node : static_objs )
    {
        auto obj = node.GetObject();
        auto spriteObj = obj.FindMember( "sprite" )->value.GetArray();

        EntityPrefab prefab;

        prefab.name = obj.FindMember( "name" )->value.GetString();
        prefab.spritesheetName = spriteObj[0].GetString();
        prefab.spriteName = spriteObj[1].GetString();
        prefab.passible = obj.FindMember( "passable" )->value.GetBool();

        std::string typeStr = obj.FindMember( "type" )->value.GetString();
        if ( typeStr == "decor" )
        {
            prefab.prefabType = PrefabEntityType::Decor;
        }
        else if ( typeStr == "door" )
        {
            prefab.prefabType = PrefabEntityType::Door;
        }
        else if ( typeStr == "container" )
        {
            prefab.prefabType = PrefabEntityType::Container;
        }
        else if ( typeStr == "trap" )
        {
            prefab.prefabType = PrefabEntityType::Trap;
        }
        else
        {
            AssertAlways();
        }

        m_prefabs.emplace( prefab.name, prefab );

    }
}

EntityRef EntityFactory::createPrefabByName(Vector2i pos, std::string const &name) const
{
    auto eref = m_parent->createEntity();

    auto const& prefab = m_prefabs.at(name);

    m_parent->addComponent<Components::TilePosition>(eref, pos);

    auto sprite = ResourceManager::get().getResource<SpritesheetResource>( prefab.spritesheetName )
            ->get()->spriteFromName( prefab.spriteName );
    m_parent->addComponent<Components::Render>(eref, sprite);

    if ( !prefab.passible )
    {
        m_parent->addComponent<Components::Collider>(eref);
    }

    m_parent->entityReady( eref );
    return eref;
}

