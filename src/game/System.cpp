#include <game/System.h>
#include <game/Component.h>
#include <game/ECS.h>
#include <graphics/RenderInterface.h>
#include <resource/Sprite.h>

System::System(ECS *parent) : m_ecs(parent)
{

}

void System::update(uint32_t ticks, RenderInterface &rInter)
{

}


Systems::Render::Render(ECS *parent) : System(parent)
{

}

void Systems::Render::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile] : m_ecs->with<Components::Render, Components::TilePosition>() )
    {
        rInter.addWorldItem(render->sprite.renderObject(tile->position));
    }
}

