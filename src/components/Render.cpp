#include <components/Render.h>

Components::Render::Render(std::vector<Sprite> const &ss)
    : sprites(ss), current(0)
{
    for ( auto &s : sprites )
    {
        s.setRenderLayer(RenderLayer::Entity);
    }
}


Components::Render::Render(Sprite const &s)
    : Components::Render::Render( std::vector<Sprite>{s} )
{
}
