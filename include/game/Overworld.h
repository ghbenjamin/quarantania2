#pragma once

#include <vector>
#include <unordered_map>

#include <game/GameEventHub.h>
#include <game/AnimationQueue.h>
#include <utils/Containers.h>

// Forward definitions
class RenderInterface;
class InputInterface;
class RandomState;
struct IEvent;

enum class OverworldLocationType
{
    Fight,
    Shop,
    Mystery,
    Boss
};

enum class OverworldExitStatus
{
    None,
    Completed,
    MainMenu,
    Desktop
};

struct OverworldLocation
{
    OverworldLocationType type;
    Vector2i gridPos;
    Vector2i displayOffset;
    bool completed = false;
    bool visitable = false;
};

struct OverworldData
{
    Vector2i gridSize;
    std::vector<OverworldLocation> locations;
    std::unordered_multimap<Vector2i, Vector2i, Vector2iHash> connections;
};


class Overworld
{
public:

    Overworld(OverworldData const& data, RandomState* randomState);
    ~Overworld() = default;

    bool input(IEvent &evt);
    void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    
    GameEventHub& events();
    Vector2i const& gridSize() const;
    std::vector<OverworldLocation> const& locations();
    std::unordered_multimap<Vector2i, Vector2i, Vector2iHash> const& connections();
    
private:
    OverworldData m_data;
    AnimationQueue m_animation;
    GameEventHub m_gevents;
    RandomState* m_randomState;
};