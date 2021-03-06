#include <memory>

#include <engine/Engine.h>
#include <graphics/Window.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <utils/Time.h>
#include <utils/GlobalConfig.h>
#include <state/LevelState.h>
#include <state/InitState.h>
#include <engine/InputInterface.h>

void Engine::run()
{
//#define FPS_COUNTER
#ifdef FPS_COUNTER
    
    double fpsTotal = 0;
    double fpsTicks = 0;
#endif

    GlobalConfig::GlobalConfigInfo globalConfig = GlobalConfig::load( "../config.json" );
    const uint32_t msPerFrame = 1000 / globalConfig.maxFPS;
    
    auto window = std::make_shared<Window>( globalConfig );
    
    ResourceManager::get().loadAll();
    ResourceManager::get().setWindow( window );

    Renderer renderer( window->getSize().convert<float>() );
    window->initResources();

    RenderInterface renderInterface ( &renderer );
    InputInterface inputInterface;

    auto initState = std::make_unique<InitState>();
    m_states.push_back( std::unique_ptr<GameState>( initState.release() ));

    auto timer = Timer();
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
            if (ievt.type == IEventType::WindowResize)
            {
                window->onWindowResize( ievt.windowResize.screenSize );
                renderer.setWindowSize( ievt.windowResize.screenSize.convert<float>() );
            }
        
            m_states.back()->input(ievt);
        }

        inputInterface.clear();


        // Update
        m_states.back()->update(ticks, inputInterface, renderInterface);
        renderer.update(ticks);

        // Graphics
        renderer.render();
        window->swapWindow();

        // State checking

        if ( m_states.back()->hasRequestedExit() )
        {
            // The current state has requested that we exit the program
            runGame = false;
        }
        
        else if (m_states.back()->hasRequestedPopState() )
        {
            // Clear the current state if we've been asked to
            m_states.pop_back();
        }
        
        else if ( m_states.back()->hasNextState() )
        {
            // There's a new state. Should we add or replace?
            if (m_states.back()->hasRequestedReplaceState())
            {
                // Replace the current state with the new one.
                auto next = std::move( m_states.back()->getNextState() );
                m_states.pop_back();
                m_states.push_back( std::move(next) );
            }
            else
            {
                m_states.push_back( m_states.back()->getNextState() );
            }
        }

        // Rely on vsync for now to manage the framerate rather than hacking about with sleeps
        ticks = timer.elapsed();

#ifdef FPS_COUNTER
        fpsTicks++;
        fpsTotal += ticks;
        
        
        if (fpsTotal > 2000)
        {
            double ticksPerFrame = fpsTotal / fpsTicks;
            double framesPerSecond = 1000 / ticksPerFrame;
            
            fpsTicks = 0;
            fpsTotal = 0;
            
            Logging::log( fmt::format( "FPS: {}", framesPerSecond) );
        }
#endif
    }

    // Cleanup
    ResourceManager::get().unloadAll();
}