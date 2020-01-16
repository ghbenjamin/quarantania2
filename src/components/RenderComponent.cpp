#include <components/RenderComponent.h>

RenderComponent::RenderComponent(std::vector<Sprite> const &ss)
    : sprites(ss), current(0)
{
    for ( auto &s : sprites )
    {
        s.setRenderLayer(RenderLayer::Entity);
    }
}


RenderComponent::RenderComponent(Sprite const &s)
    : RenderComponent::RenderComponent( std::vector<Sprite>{s} )
{
}
