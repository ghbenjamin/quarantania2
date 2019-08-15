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
        PrefabList pcl;

        auto nodeObj = node.GetObject();
        std::string_view nodeName = nodeObj.FindMember("name")->value.GetString();
        auto nodeComps = nodeObj.FindMember("components")->value.GetObject();

        for ( auto const& comp : nodeComps )
        {
            std::string_view compName = comp.name.GetString();

            if ( compName == "render" )
            {
                Prefab::Component::Render c;
                c.renderStates = comp.value.GetObject().FindMember("render-states")->value.GetInt();

                for ( auto const& tileState : comp.value.GetObject().FindMember("tilesets")->value.GetArray() )
                {
                    for ( auto const& tileSet : tileState.GetArray() )
                    {
                        auto tsObj = tileSet.GetObject();
                        c.sprites.emplace_back( tsObj.FindMember("sheet" )->value.GetString(),
                                                tsObj.FindMember("tile" )->value.GetString() );
                    }
                }

                pcl.push_back( c );
            }
            else if ( compName == "collider" )
            {
                Prefab::Component::Collider c;
                pcl.push_back( c );
            }
            else if ( compName == "state" )
            {
                Prefab::Component::State c;
                pcl.push_back( c );
            }
            else if ( compName == "container" )
            {
                Prefab::Component::Container c;
                pcl.push_back( c );
            }
            else
            {
                Logging::log("WARN: unexpected component name: {}", compName );
            }
        }

        m_prefabs[std::string(nodeName)] = pcl;
    }
}

EntityRef EntityFactory::createPrefabByName(Vector2i pos, std::string const &name) const
{
    auto eref = m_parent->createEntity();

//    auto const& prefab = m_prefabs.at(name);
//
//    m_parent->addComponent<Components::TilePosition>(eref, pos);
//
//    auto sprite = ResourceManager::get().getResource<SpritesheetResource>( prefab.spritesheetName )
//            ->get()->spriteFromName( prefab.spriteName );
//    m_parent->addComponent<Components::Render>(eref, sprite);
//
//    if ( !prefab.passible )
//    {
//        m_parent->addComponent<Components::Collider>(eref);
//    }
//
//    m_parent->entityReady( eref );
    return eref;
}

