#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

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
    int idx;
    Vector2i gridPos;
    Vector2i displayOffset;
    bool completed = false;
    bool visitable = false;
};

struct OverworldData
{
    Vector2i gridSize;
    std::vector<OverworldLocation> locations;
    std::unordered_multimap<int, int> connections;
    int currentLocation;
    std::unordered_set<int> rootNodes;
};


class Overworld
{
public:
    Overworld(std::shared_ptr<OverworldData> const& data, RandomState* randomState);
    ~Overworld() = default;

    bool input(IEvent &evt);
    void update(std::uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    
    GameEventHub& events();
    Vector2i const& gridSize() const;
    std::vector<OverworldLocation> const& locations();
    std::unordered_multimap<int, int> const& connections();
    int currentLocation() const;
    std::unordered_set<int> const& nextLocations() const;
    
    std::shared_ptr<OverworldData> data();
    
private:
    std::shared_ptr<OverworldData> m_data;
    
    AnimationQueue m_animation;
    GameEventHub m_gevents;
    RandomState* m_randomState;
    std::unordered_set<int> m_allowedNextLocs;
};