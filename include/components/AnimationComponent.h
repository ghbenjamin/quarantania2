#pragma once

#include <vector>
#include <optional>

#include <utils/Containers.h>
#include <engine/Component.h>
#include <utils/Interpolate.h>

class TileAnimationPath
{
public:
    TileAnimationPath( std::vector<Vector2f> const &path, float timePerStep );
    
    void advance( std::uint32_t ticks );
    
    Vector2f currentPosition() const;
    Vector2f finalPosition() const;
    bool isComplete() const;
    
private:
    
    TimedLinearInterpolator<Vector2f> m_interpolator;
    
    std::vector<Vector2f> m_path;
    
    float m_timePerStep;
    int m_currIdx;
    bool m_isComplete;
};

struct AnimationComponent : public Component<AnimationComponent>
{
    AnimationComponent() = default;
    ~AnimationComponent() override = default;
    
    std::optional<TileAnimationPath> movementPathAnim;
};
