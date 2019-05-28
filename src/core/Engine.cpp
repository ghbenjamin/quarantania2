
#include <memory>
#include <iostream>

#include <core/Engine.h>
#include <graphics/Window.h>
#include <graphics/RenderInterface.h>
#include <resource/Manifest.h>
#include <resource/ResourceManager.h>
#include <resource/Sprite.h>
#include <utils/Time.h>
#include <utils/Logging.h>
#include <utils/GlobalConfig.h>
#include <state/State.h>
#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/InputInterface.h>

void Engine::run()
{
    GlobalConfig::GlobalConfigInfo globalConfig = GlobalConfig::load( "../config.json" );
    const uint32_t msPerFrame = 1000 / globalConfig.maxFPS;

    auto window = std::make_shared<Window>(
        globalConfig.windowTitle, globalConfig.screenSize
    );

    ResourceManager::get().setWindow( window );
    ResourceManager::get().registerAll("../resource/manifest.json");
    ResourceManager::get().loadAll();

    RenderInterface renderInterface {window};
    renderInterface.camera().setBounds( {1000, 1000} );
    renderInterface.camera().setViewportSize( window->getSize() );

    InputInterface inputInterface;

    LevelConfig debugConfig;
    debugConfig.size = {75, 55};

    pushState<LevelState>( debugConfig );

    Timer timer = Timer();
    uint32_t ticks = 0;
    bool runGame = true;

    while (runGame)
    {
        timer.start();

        // Input

        SDL_Event evt;
        while (SDL_PollEvent(&evt) != 0)
        {
            if (evt.type == SDL_QUIT)
            {
                runGame = false;
            }
            else
            {
                inputInterface.input(evt);
            }
        }

        for ( auto& ievt : inputInterface.queue() )
        {
            m_states.back()->input(ievt);
        }

        inputInterface.clear();


        // Update

        renderInterface.clear();
        m_states.back()->update(ticks, inputInterface, renderInterface);

        // Graphics

        window->renderer()->render(renderInterface);

        // Hacky framerate limiting
        ticks = timer.elapsed();
        if (ticks < msPerFrame)
        {
            SDL_Delay(msPerFrame - ticks);
        }
    }

    // Cleanup
    ResourceManager::get().unloadAll();
}

GameStatePtr Engine::popState()
{
    auto ptr = std::move(m_states.back());
    m_states.pop_back();
    return std::move(ptr);
}
