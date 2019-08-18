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
    auto const& prefabComponents = m_prefabs.at(name);

    m_parent->addComponent<Components::TilePosition>(eref, pos);

    for ( auto const& pc : prefabComponents )
    {
        std::visit( Prefab::Visitor{m_parent, eref}, pc );
    }

    m_parent->entityReady( eref );

    return eref;
}

Prefab::Visitor::Visitor(Level* level, EntityRef ref)
: m_ref(ref), m_level(level) {}

void Prefab::Visitor::operator()(Component::Render const& obj) const
{
    // TODO Don't always take the first one
    auto sprite = ResourceManager::get().getSprite(obj.sprites.front());
    m_level->addComponent<Components::Render>(m_ref, sprite);
}
void Prefab::Visitor::operator()(Component::State const& obj) const
{
}
void Prefab::Visitor::operator()(Component::Collider const& obj) const
{
    m_level->addComponent<Components::Collider>(m_ref);
}
void Prefab::Visitor::operator()(Component::Container const& obj) const
{
}