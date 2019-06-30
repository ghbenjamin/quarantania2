#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>

EntityFactory::EntityFactory(Level *parent)
: m_parent(parent)
{
}


std::unique_ptr<Player> EntityFactory::createPlayer(ImPlayerData &data, Vector2i startPos) const
{
    auto eref = m_parent->createEntity();

    m_parent->addComponent<Components::TilePosition>(eref, startPos);

    auto sprite = ResourceManager::get().getResource<SpritesheetResource>( "kenney-chars" )
        ->get()->spriteFromName( "example-char-1" );
    m_parent->addComponent<Components::Render>(eref, sprite);

    m_parent->addComponent<Components::Collider>(eref);


    return std::make_unique<Player>( std::move(data), eref );
}

EntityRef EntityFactory::debugHighlight(Vector2i pos, std::string const& tile) const
{
    auto eref = m_parent->createEntity();

    m_parent->addComponent<Components::TilePosition>(eref, pos);

    auto sprite = ResourceManager::get().getResource<SpritesheetResource>( "kenney-tiles" )
            ->get()->spriteFromName( tile );
    m_parent->addComponent<Components::Render>(eref, sprite);

    m_parent->entityReady( eref );
    return eref;
}

EntityRef EntityFactory::createDoor(Vector2i pos) const
{
    auto eref = m_parent->createEntity();

    m_parent->addComponent<Components::TilePosition>(eref, pos);

    auto sprite = ResourceManager::get().getResource<SpritesheetResource>( "kenney-tiles" )
        ->get()->spriteFromName( "door-1" );
    m_parent->addComponent<Components::Render>(eref, sprite);

    m_parent->addComponent<Components::Collider>(eref);

    m_parent->entityReady( eref );
    return eref;
}

