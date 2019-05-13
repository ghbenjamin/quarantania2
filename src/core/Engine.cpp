
#include <memory>

#include <core/Engine.h>
#include <graphics/Window.h>
#include <utils/Time.h>
#include <resource/Manifest.h>
#include <iostream>
#include <resource/ResourceManager.h>
#include <utils/GlobalConfig.h>
#include <graphics/RenderInterface.h>
#include <state/State.h>
#include <state/LevelState.h>
#include <game/LevelConfig.h>

void Engine::run()
{
    GlobalConfig::GlobalConfigInfo globalConfig = GlobalConfig::load( "../config.json" );
    const Uint32 msPerFrame = 1000 / globalConfig.maxFPS;

    auto window = std::make_shared<Window>(
        globalConfig.windowTitle, globalConfig.screenSize
    );

    ResourceManager::get().setWindow( window );
    ResourceManager::get().registerAll("../resource/manifest.json");
    ResourceManager::get().loadAll();

    auto ptr = ResourceManager::get().getResource<SpritesheetResource>( "kenney-chars" )->get();
    auto dbg = ptr->sheetPosFromGid( 271 );

    RenderInterface renderInterface;
    LevelConfig debugConfig;

    pushState<LevelState>( debugConfig );

    Timer timer = Timer();
    Uint32 ticks = 0;
    bool runGame = true;

    while (runGame)
    {
        timer.start();
        renderInterface.clear();

        SDL_Event evt;
        while (SDL_PollEvent(&evt) != 0)
        {
            if (evt.type == SDL_QUIT)
            {
                runGame = false;
            }

            m_states.back()->input(evt);
        }

        m_states.back()->update(ticks, renderInterface);

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
