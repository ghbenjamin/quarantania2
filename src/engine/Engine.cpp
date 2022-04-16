#include <memory>

#include <state/GameState.h>
#include <engine/Engine.h>
#include <graphics/Window.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>
#include <utils/Time.h>
#include <utils/GlobalConfig.h>
#include <state/InitState.h>
#include <engine/InputInterface.h>
#include <utils/Memory.h>
#include <engine/LuaState.h>

void Engine::run()
{
    LuaState luaState;

    GlobalConfig::GlobalConfigInfo globalConfig = GlobalConfig::load( "../config.lua", luaState );

    // Load all of our resources - TODO defer loading if necessary
    auto window = std::make_shared<Window>( globalConfig );

    ResourceManager::get().loadAll();
    ResourceManager::get().setWindow( window );

    Renderer renderer( window->getSize().convert<float>() );
    window->initResources();

    RenderInterface renderInterface ( &renderer );
    InputInterface inputInterface;

    auto initState = utils::make_unique_with_type<GameState, InitState>( luaState );
    m_states.push_back( std::move(initState) );


    // Start the main game loop

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

        bool stateChanged = false;

        if ( m_states.back()->hasRequestedExit() )
        {
            // The current state has requested that we exit the program
            runGame = false;
        }
        else
        {
            auto nextState = std::move( m_states.back()->getPushedState() );

            if (m_states.back()->hasPoppedState() )
            {
                // Clear the current state if we've been asked to
                m_states.pop_back();
                stateChanged = true;
            }

            if ( nextState )
            {
                // Add another state if we've been asked to
                m_states.push_back( std::move(nextState) );
                stateChanged = true;
            }

            if (stateChanged)
            {
                renderer.reset();
            }
        }

        // Rely on vsync for now to manage the framerate rather than hacking about with sleeps
        ticks = timer.elapsed();
    }

    // Cleanup
    ResourceManager::get().unloadAll();
}