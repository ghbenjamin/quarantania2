
#include <memory>

#include <core/Engine.h>
#include <resource/Window.h>
#include <utils/Time.h>
#include <resource/Manifest.h>
#include <iostream>
#include <resource/ResourceManager.h>

void Engine::run()
{
    bool runGame = true;
    Timer timer = Timer();
    Uint32 ticks = 0;
    const Vector2i screenSize = { 800, 600 };
    const int maxFPS = 60;
    const Uint32 msPerFrame = 1000 / maxFPS;

    auto window = std::make_unique<Window>( "Quarantania", screenSize );

    ResourceManager resources;
    resources.loadAll( window );


    while (runGame)
    {
        timer.start();

        SDL_Event evt;
        while (SDL_PollEvent(&evt) != 0)
        {
            if (evt.type == SDL_QUIT)
            {
                runGame = false;
            }
        }

        // Hacky framerate limiting
        ticks = timer.elapsed();
        if (ticks < msPerFrame)
        {
            SDL_Delay(msPerFrame - ticks);
        }
    }
}
