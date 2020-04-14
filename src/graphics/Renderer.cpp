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
    SDL_Rect const* clipRect = NULL;

    for ( auto const& item : objs.renderables() )
    {
        if ( !SDL_RectEmpty( &item.clipRect ) )
        {
            if ( !SDL_RectEquals( clipRect, &item.clipRect ) )
            {
                // If this item has a clip rect, and it's different to our current clip rect, set the cliprect
                SDL_RenderSetClipRect( m_renderer, &item.clipRect );
                clipRect = &item.clipRect;
            }
        }
        else if ( clipRect != NULL )
        {
            // Otherwise, if the requested clip is null, and the current clip is not, remove the clip
            clipRect = NULL;
            SDL_RenderSetClipRect( m_renderer, NULL );
        }

        // If the current clip is the same as the requested clip, do nothing

        // Render the item, respecting the above clip
        SDL_RenderCopy(m_renderer, item.texture, &item.sourceRect, &item.targetRect);
    }

    SDL_RenderPresent( m_renderer );
}
