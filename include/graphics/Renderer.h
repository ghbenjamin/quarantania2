#pragma once

#include <SDL2/SDL.h>
#include <memory>

class RenderInterface;

class Renderer
{
public:

    explicit Renderer( SDL_Window* target );
    ~Renderer();

    Renderer( const Renderer& ) = delete;
    Renderer& operator=( const Renderer& ) = delete;

    SDL_Renderer* raw() const;

    void render( RenderInterface const& objs );


private:
    SDL_Renderer* m_renderer;

};

using RendererPtr = std::unique_ptr<Renderer>;