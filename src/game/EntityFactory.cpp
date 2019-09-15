#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <rapidjson/document.h>
#include <utils/Json.h>

EntityFactory::EntityFactory(Level *parent, RandomGenerator* rg )
: m_parent(parent), m_rg(rg)
{
    m_prefabDecorators.emplace( "door", &EntityFactory::createDoor );
    m_prefabDecorators.emplace( "entrance", &EntityFactory::createEntrance );
    m_prefabDecorators.emplace( "exit", &EntityFactory::createExit );
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

                auto tilesetArray = comp.value.GetObject().FindMember("tilesets")->value.GetArray();
                c.renderStates = tilesetArray.Size();

                for ( auto const& tileState : tilesetArray )
                {
                    auto tileStateArray = tileState.GetArray();
                    c.spriteBreakpoints.push_back( tileStateArray.Size() );

                    for ( auto const& tileSet : tileStateArray )
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
                auto arr = comp.value.GetArray();
                for ( auto const& a : arr )
                {
                    c.states.push_back( a.GetString() );
                }
                pcl.push_back( c );
            }
            else if ( compName == "container" )
            {
                Prefab::Component::Container c;
                pcl.push_back( c );
            }
            else if ( compName == "description" )
            {
                Prefab::Component::Description c;

                auto descArray = comp.value.GetArray();
                for ( auto const& desc : descArray )
                {
                    c.descriptions.emplace_back( desc.GetString() );
                }

                pcl.push_back( c );
            }
            else
            {
                Logging::log("WARN: unexpected component name: {}\n", compName );
            }
        }

        m_prefabs[std::string(nodeName)] = pcl;
    }
}

EntityRef EntityFactory::createPrefabByName(std::string const &name, Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto const& prefabComponents = m_prefabs.at(name);

    // All prefabs have a position
    m_parent->addComponent<Components::TilePosition>(eref, pos);

    // Construct the simple components
    for ( auto const& pc : prefabComponents )
    {
        std::visit( Prefab::Visitor{m_parent, eref, m_rg}, pc );
    }

    // Construct the components which are specific to specific types of prefab
    if ( m_prefabDecorators.find(name) != m_prefabDecorators.end() )
    {
        auto mptr = m_prefabDecorators.at(name);
        ( this->*mptr ) (eref);
    }

    m_parent->entityReady( eref );

    return eref;
}

void EntityFactory::createDoor(EntityRef ref) const
{
}

void EntityFactory::createEntrance(EntityRef ref) const
{
}

void EntityFactory::createExit(EntityRef ref) const
{
}

Prefab::Visitor::Visitor(Level* level, EntityRef ref, RandomGenerator* rg)
: m_ref(ref), m_level(level), m_rg(rg) {}

void Prefab::Visitor::operator()(Component::Render const& obj) const
{
    // TODO: something more sophisticated than randomly picking

    auto begin_it = obj.sprites.begin();
    auto end_it = obj.sprites.begin();
    std::advance(end_it, obj.spriteBreakpoints[0]);

    auto it = randomElement(begin_it, end_it, *m_rg);
    auto sprite = ResourceManager::get().getSprite(*it);

    m_level->addComponent<Components::Render>(m_ref, sprite);
}

void Prefab::Visitor::operator()(Component::State const& obj) const
{
    m_level->addComponent<Components::FixedState>(m_ref, obj.states);
}

void Prefab::Visitor::operator()(Component::Collider const& obj) const
{
    m_level->addComponent<Components::Collider>(m_ref);
}

void Prefab::Visitor::operator()(Component::Container const& obj) const
{
}

void Prefab::Visitor::operator()(Component::Description const& obj) const
{
    m_level->addComponent<Components::Description>(m_ref, obj.descriptions);
}