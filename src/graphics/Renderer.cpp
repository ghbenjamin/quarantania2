#include <graphics/Renderer.h>
#include <graphics/RenderInterface.h>
#include <utils/Assert.h>

Renderer::Renderer( SDL_Window* target )
{
    m_renderer = SDL_CreateRenderer(
        target,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

//    SDL_RenderSetScale( m_renderer, 2.0f, 2.0f );

    Assert( m_renderer != nullptr );
}

Renderer::~Renderer()
{
    if ( m_renderer )
    {
        SDL_DestroyRenderer(m_renderer);
    }
}

SDL_Renderer *Renderer::raw() const
{
    return m_renderer;
}

void Renderer::render(RenderInterface const &objs)
{
    SDL_RenderClear( m_renderer );
    SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND );

    for ( auto const& item : objs.renderables() )
    {
        SDL_RenderCopy(m_renderer, item.texture, &item.sourceRect, &item.targetRect);
    }

    SDL_RenderPresent( m_renderer );
}
