#include <game/EntityFactory.h>

#include <game/Level.h>
#include <game/ActorModFactory.h>
#include <resource/ResourceManager.h>
#include <components/All.h>
#include <components/AnimationComponent.h>
#include <ai/BehaviourNodes.h>
#include <ai/EntityTargetingScheme.h>
#include <ai/BehaviourFactory.h>
#include <game/RunState.h>

EntityFactory::EntityFactory(Level* parent)
: m_parent(parent) {}


EntityRef EntityFactory::createPlayer(Vector2i pos, PlayerData const &data) const
{
    auto eref = m_parent->ecs().createEntity();
    auto sprite = ResourceManager::get().getSprite(data.sprite);

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);
    m_parent->ecs().addComponent<ColliderComponent>(eref, false, true);
    m_parent->ecs().addComponent<AnimationComponent>(eref);

    auto cContainer = m_parent->ecs().addComponent<ContainerComponent>(eref);

    auto actor = Actor(m_parent, eref, data);
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
        auto slot = cActor->actor.defaultSlotForItemSlot( ptr->getEquipSlot() );
        if (slot)
        {
            cActor->actor.equipItem( *slot, ptr );
        }
    }
    
    for ( auto const& featId : data.featIds )
    {
        cActor->actor.addModifierGroup( ActorModFactory::fromId(featId) );
    }

    return eref;
}

EntityRef EntityFactory::createEnemy(Vector2i pos, std::string const &name) const
{
    auto eref = m_parent->ecs().createEntity();
    auto creatureData = ResourceDatabase::instance().creatureFromName( name );
    auto sprite = ResourceManager::get().getSprite( creatureData.sprite );

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);
    m_parent->ecs().addComponent<ColliderComponent>(eref, false, true);
    m_parent->ecs().addComponent<DescriptionComponent>( eref, creatureData.name, "Creature", creatureData.description );
    m_parent->ecs().addComponent<AnimationComponent>(eref);
    
    auto aiC = m_parent->ecs().addComponent<AIComponent>(eref);
    aiC->behaviour = BehaviourFactory::moveAndAttackNearestEnemy();

    auto actor = Actor( m_parent, eref, creatureData );
    auto actComp = m_parent->ecs().addComponent<ActorComponent>(eref, std::move(actor));
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

//        auto actions = m_parent->ecs().addComponent<ActionComponent>(eref);
//        actions->actions.push_back( std::make_shared<ExitLevelAction>(m_parent) );
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
        const std::unordered_map<std::string, JSONValue> &data) const
{
    auto eref = m_parent->ecs().createEntity();

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);

    auto spriteRef = ResourceManager::get().getSprite( sprite );
    m_parent->ecs().addComponent<RenderComponent>(eref, spriteRef);


    // Handle any objects whose behaviour is specified by name
    if ( name == "door" )
    {
        m_parent->ecs().addComponent<ColliderComponent>(eref, true, true);
        m_parent->ecs().addComponent<OpenableComponent>(eref);
    }
   
   
    // Handle objects whose behaviour is componentized
    
    // Handle lighting component
    auto lightIntensity = data.find("light-intensity");
    if ( lightIntensity != data.end() )
    {
        float intensityVal = std::get<float>(lightIntensity->second);
        std::string colourVal;
        
        auto lightColour = data.find("light-colour");
        if (lightColour != data.end())
        {
            colourVal = std::get<std::string>(lightColour->second);
        }
        else
        {
            colourVal = "#ffffffff";
        }
    
        m_parent->ecs().addComponent<LightingComponent>(eref, intensityVal, Colour(colourVal));
    }
   

    m_parent->ecs().entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createDecor(Vector2i pos, SpritesheetKey const& key) const
{
    auto eref = m_parent->ecs().createEntity();
    auto sprite = ResourceManager::get().getSprite( key );

    m_parent->ecs().addComponent<PositionComponent>(eref, pos);
    m_parent->ecs().addComponent<RenderComponent>(eref, sprite);

    m_parent->ecs().entityReady(eref);
    return eref;
}

EntityRef EntityFactory::createCorpse(EntityRef actorRef)
{
    auto eref = m_parent->ecs().createEntity();

    auto const&[ actorC, renderC, posC ] =  m_parent->ecs().getComponents<
            ActorComponent, RenderComponent, PositionComponent>(actorRef);

    m_parent->ecs().addComponent<PositionComponent>(eref, posC->tilePosition);

    auto newRenderC = m_parent->ecs().addComponent<RenderComponent>(eref, renderC->sprite);
    newRenderC->sprite.resetColourMod();
    newRenderC->sprite.setAlphaMod(0.5f);

    return eref;
}


