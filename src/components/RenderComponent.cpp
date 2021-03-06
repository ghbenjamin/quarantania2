#include <components/RenderSystem.h>

RenderComponent::RenderComponent(std::vector<Sprite> const& sprites)
    : sprites(sprites), current(0)
{
    for ( auto &s : this->sprites )
    {
        s.setRenderLayer(RenderLayer::Entity);
    }
}


RenderComponent::RenderComponent(Sprite const &s)
    : RenderComponent::RenderComponent( std::vector<Sprite>{s} ) {}
