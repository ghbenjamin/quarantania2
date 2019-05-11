
#include <memory>

#include <core/Engine.h>
#include <graphics/Window.h>
#include <utils/Time.h>
#include <resource/Manifest.h>
#include <iostream>
#include <resource/ResourceManager.h>
#include <utils/GlobalConfig.h>

void Engine::run()
{
    GlobalConfig::GlobalConfigInfo globalConfig = GlobalConfig::load( "../config.json" );

    auto window = std::make_unique<Window>(
        globalConfig.windowTitle, globalConfig.screenSize
    );

    ResourceManager resources;
    resources.loadAll( window );


    bool runGame = true;
    Timer timer = Timer();
    Uint32 ticks = 0;
    const Uint32 msPerFrame = 1000 / globalConfig.maxFPS;

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
