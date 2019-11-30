#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <rapidjson/document.h>
#include <components/All.h>

EntityFactory::EntityFactory(Level *parent, RandomGenerator* rg )
: m_parent(parent), m_rg(rg)
{
    createPrefabs();
}

std::unique_ptr<Player> EntityFactory::createPlayer(ImPlayerData &data, Vector2i startPos) const
{
    auto eref = m_parent->createEntity();

    auto sprite = ResourceManager::get().getSprite("kenney-chars", "example-char-1");
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->addComponent<Components::TilePosition>(eref, startPos);
    m_parent->addComponent<Components::Render>(eref, sprite);
    m_parent->addComponent<Components::Collider>(eref);
    m_parent->addComponent<Components::Actor>(eref);

    return std::make_unique<Player>( std::move(data), eref );
}

EntityRef EntityFactory::createPrefabByName(std::string const &name, Vector2i pos) const
{
    auto eref = m_parent->createEntity();
    auto it = m_prefabs.find(name);

    // TODO Do we always want a tile position?
    m_parent->addComponent<Components::TilePosition>(eref, pos);

    Assert( it != m_prefabs.end() );

    // TODO Don't just use the first one!
    auto& chosen = it->second.front();
    chosen->generate(m_parent, eref);

    m_parent->entityReady( eref );

    return eref;
}

void EntityFactory::createPrefabs()
{
    using ContainerType = std::vector<std::shared_ptr<PrefabObj>>;

    m_prefabs.emplace( "door", ContainerType {
        std::make_shared<PrefabObjs::Door>( SpritesheetKey{ "kenney-tiles", "door-closed" } ),
        std::make_shared<PrefabObjs::Door>( SpritesheetKey{ "kenney-tiles", "door-barred" } ),
        std::make_shared<PrefabObjs::Door>( SpritesheetKey{ "kenney-tiles", "door-open" } )
    });

    m_prefabs.emplace( "entrance", ContainerType {
        std::make_shared<PrefabObjs::Entrance>( SpritesheetKey{ "kenney-tiles", "grey-stairs-up" } )
    });

    m_prefabs.emplace( "exit", ContainerType {
        std::make_shared<PrefabObjs::Exit>( SpritesheetKey{ "kenney-tiles", "grey-stairs-down" } )
    });

    m_prefabs.emplace( "container", ContainerType {
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "bookcase-medium-full"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "bookcase-small-empty"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "bookcase-small-full"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "bookcase-medium-empty"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "bookcase-large-empty"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "bookcase-large-full"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "barrel-open"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "barrel-closed"} ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "single-wide-drawer" } ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "double-wide-drawer" } ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "single-thin-drawer" } ),
        std::make_shared<PrefabObjs::Container>( SpritesheetKey{ "kenney-tiles", "double-thin-drawer" } )
    });

    m_prefabs.emplace( "decor", ContainerType {
        std::make_shared<PrefabObjs::Decor>( SpritesheetKey{ "kenney-tiles", "bed-made" } ),
        std::make_shared<PrefabObjs::Decor>( SpritesheetKey{ "kenney-tiles", "bed-unmade" } ),
        std::make_shared<PrefabObjs::Decor>( SpritesheetKey{ "kenney-tiles", "chair-left" } ),
        std::make_shared<PrefabObjs::Decor>( SpritesheetKey{ "kenney-tiles", "chair-right" } )
    });
}