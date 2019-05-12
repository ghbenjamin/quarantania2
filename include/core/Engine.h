

#include <state/State.h>
#include <vector>
#include <memory>

class Engine
{
public:

    Engine() = default;
    ~Engine() = default;

    Engine( const Engine& ) = delete;
    Engine& operator=( const Engine& ) = delete;

    void run();


    template <typename T, typename... Args>
    void pushState(Args... args)
    {
        auto dptr = std::make_unique<T>(std::forward<Args>(args)...);
        auto bptr = std::unique_ptr<GameState>( dptr.release() );
        m_states.push_back( std::move(bptr) );
    }

    GameStatePtr popState();


private:
    std::vector<GameStatePtr> m_states;

};