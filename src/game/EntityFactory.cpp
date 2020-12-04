#include <game/EntityFactory.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <components/All.h>
#include <components/ContainerComponent.h>
#include <game/ResourceDatabase.h>
#include <game/ActionDefs.h>

EntityFactory::EntityFactory(Level* parent)
: m_parent(parent) {}


EntityRef EntityFactory::createPlayer(Vector2i pos, PlayerData const &data) const
{
    auto eref = m_parent->ecs().createEntity();

    auto sprite = ResourceManager::get().getSprite(data.sprite);
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);
    m_parent->ecs().addComponent<ColliderComponent>(eref, false, true);
    m_parent->ecs().addComponent<PCComponent>(eref);

    auto cContainer = m_parent->ecs().addComponent<ContainerComponent>(eref);

    auto actor = Actor(data);
    auto cActor = m_parent->ecs().addComponent<ActorComponent>(eref, std::move(actor));
    cActor->actorType = ActorType::PC;

    m_parent->ecs().entityReady(eref);

    for ( auto const& iname : data.startingHeldItems )
    {
        cContainer->items.push_back( Item::fromName( iname ) );
    }

    for ( auto const& iname : data.startingEquippedItems )
    {
        auto ptr = Item::fromName( iname );
        cActor->actor.equipItem( ptr->getEquipSlot(), ptr );
    }

    return eref;
}

EntityRef EntityFactory::createEnemy(Vector2i pos, std::string const &name) const
{
    auto eref = m_parent->ecs().createEntity();
    auto creatureData = ResourceDatabase::instance().creatureFromName( name );

    auto sprite = ResourceManager::get().getSprite( creatureData.sprite );
    sprite.setRenderLayer(RenderLayer::Actor);

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<AIComponent>(eref);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);
    m_parent->ecs().addComponent<ColliderComponent>(eref, false, true);
    m_parent->ecs().addComponent<DescriptionComponent>( eref, creatureData.name, "Creature", creatureData.description );

    auto actComp = m_parent->ecs().addComponent<ActorComponent>(eref, std::move(creatureData));
    actComp->actorType = ActorType::NPC;

    m_parent->ecs().entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createItem(Vector2i pos, std::string const &name) const
{
    ItemPtr item = Item::fromName( name );
    return createItem(pos, item);
}

EntityRef EntityFactory::createItem(Vector2i pos, std::shared_ptr<Item> item) const
{
    auto eref = m_parent->ecs().createEntity();

    auto sprite = ResourceManager::get().getSprite( item->getSprite() );
    sprite.setRenderLayer(RenderLayer::Entity);

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);
    m_parent->ecs().addComponent<ColliderComponent>(eref, false, false);
    m_parent->ecs().addComponent<ItemComponent>(eref, item);
    m_parent->ecs().addComponent<DescriptionComponent>( eref, item->getName(), "Item", item->getDescription() );

    m_parent->ecs().entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createObject(Vector2i pos, std::string const &ptype) const
{
    auto eref = m_parent->ecs().createEntity();
    auto objData = ResourceDatabase::instance().objectFromName( ptype );

    auto sprite = ResourceManager::get().getSprite( objData.sprites[0] );
    sprite.setRenderLayer(RenderLayer::Entity);

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);

    if ( objData.type == "door" )
    {
        m_parent->ecs().addComponent<ColliderComponent>(eref, true, true);
        m_parent->ecs().addComponent<OpenableComponent>(eref);
    }
    else if ( objData.type == "level_entrance" )
    {
        m_parent->ecs().addComponent<ColliderComponent>(eref, true, true);
    }
    else if ( objData.type == "level_exit" )
    {
        m_parent->ecs().addComponent<ColliderComponent>(eref, true, true);

        auto actions = m_parent->ecs().addComponent<ActionComponent>(eref);
        actions->actions.push_back( std::make_shared<ExitLevelAction>(m_parent) );
    }
    else if ( objData.type == "container" )
    {
    }
    else if ( objData.type == "decor" )
    {
    }
    else
    {
        AssertAlways();
    }

    m_parent->ecs().entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createObject(Vector2i pos, std::string const& name, SpritesheetKey const& sprite,
        const std::unordered_map<std::string, std::string> &data) const
{
    auto eref = m_parent->ecs().createEntity();

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);

    auto spriteRef = ResourceManager::get().getSprite( sprite );
    m_parent->ecs().addComponent<RenderComponent>(eref, spriteRef);

    if ( name == "door" )
    {
        m_parent->ecs().addComponent<ColliderComponent>(eref, true, true);
        m_parent->ecs().addComponent<OpenableComponent>(eref);
    }
    else
    {

    }


    m_parent->ecs().entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createDecor(Vector2i pos, SpritesheetKey const& key) const
{
    auto eref = m_parent->ecs().createEntity();

    auto sprite = ResourceManager::get().getSprite( key );
    sprite.setRenderLayer(RenderLayer::Entity);

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);

    m_parent->ecs().entityReady(eref);
    return eref;
}


