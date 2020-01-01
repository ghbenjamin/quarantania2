#include <memory>

#include <game/Engine.h>
#include <graphics/Window.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <utils/Time.h>
#include <utils/GlobalConfig.h>
#include <state/State.h>
#include <state/LevelState.h>
#include <game/InputInterface.h>
#include <state/InitState.h>

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
    InputInterface inputInterface;

    auto initState = std::make_unique<InitState>();
    m_states.push_back( std::unique_ptr<GameState>( initState.release() ));

    auto timer = Timer();
    uint32_t ticks = 0;
    bool runGame = true;

    auto fpsTimer = Timer();
    fpsTimer.start();
    auto fpsTicks = 0;
    auto currentFps = -1;

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
        renderInterface.present();

        // Graphics

        window->renderer()->render(renderInterface);

        // State checking
        if (m_states.back()
                    ->hasRequestedPopState() )
        {
            // Clear the current state if we've been asked to
            m_states.pop_back();
        }
        else if ( m_states.back()->hasNextState() )
        {
            // Add a new state if we've been asked to
            m_states.push_back( m_states.back()->getNextState() );
        }


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