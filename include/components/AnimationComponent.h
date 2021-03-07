#pragma once

#include <vector>
#include <optional>

#include <utils/Containers.h>
#include <engine/Component.h>

class TileAnimationPath
{
public:
    TileAnimationPath( std::vector<Vector2f> const &path, double totalAnimationTime );
    
    void advance( std::uint32_t ticks );
    
    Vector2f currentPosition() const;
    bool isComplete() const;
    Vector2f finalPosition() const;
    
private:
    
    void advanceTile();

    std::vector<Vector2f> m_path;
    double m_totalAnimationTime;
    double m_timePerStep;
    
    Vector2f m_nextTile;
    Vector2f m_prevTile;
    int m_currIdx;
    
    Vector2f m_currentPxPos;
    Vector2f m_deltaPerTick;
    bool m_isComplete;
};

struct AnimationComponent : public Component<AnimationComponent>
{
    AnimationComponent() = default;
    ~AnimationComponent() override = default;
    
    
    std::optional<TileAnimationPath> movementPathAnim;
};
