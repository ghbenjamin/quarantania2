

class Engine
{
public:

    Engine() = default;
    ~Engine() = default;

    Engine( const Engine& ) = delete;
    Engine& operator=( const Engine& ) = delete;

    static void run();
};