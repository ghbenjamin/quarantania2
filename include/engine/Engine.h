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

private:
    std::vector<GameStatePtr> m_states;

};