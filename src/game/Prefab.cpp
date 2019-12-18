#include <game/Prefab.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <components/All.h>

PrefabObjs::Door::Door(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Door::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    
    level->addComponent<Components::Render>(entity, sprite);
    level->addComponent<Components::Collider>(entity, true, true);
    level->addComponent<Components::Openable>(entity);
}

PrefabObjs::Exit::Exit(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Exit::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);
}


PrefabObjs::Entrance::Entrance(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Entrance::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    level->addComponent<Components::Collider>(entity, true, true);
}

PrefabObjs::Container::Container(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Container::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    level->addComponent<Components::Collider>(entity, true, true);
}

PrefabObjs::Decor::Decor(SpritesheetKey sprite)
    : m_sprite(sprite)
{
}

void PrefabObjs::Decor::generate(Level *level, EntityRef entity)
{
    auto sprite = ResourceManager::get().getSprite(m_sprite);
    sprite.setRenderLayer(RenderLayer::Entity);
    level->addComponent<Components::Render>(entity, sprite);

    level->addComponent<Components::Collider>(entity, true, true);
}
