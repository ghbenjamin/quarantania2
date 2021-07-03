#pragma once

#include <vector>

#include <utils/Containers.h>
#include <utils/Colour.h>
#include <engine/Entity.h>
#include <utils/Interpolate.h>
#include <utils/Splines.h>

class Level;

class Animation
{
public:
    Animation( Level* level, float totalTime );
    virtual ~Animation() = default;

    void advance( std::uint32_t ticks );
    bool isComplete() const;

protected:

    virtual void advanceSelf(std::uint32_t ticks) = 0;
    virtual void onStart() {};

    Level* m_level;
    float m_totalTime;
    float m_currentTime;

    bool m_hasStarted;
    bool m_isComplete;
};


class AnimTilePath : public Animation
{
public:
    AnimTilePath( Level* level, EntityRef entity, std::vector<Vector2f> const &path, float timePerStep );

protected:
    void advanceSelf(std::uint32_t ticks) override;

private:
    EntityRef m_entity;
    std::vector<Vector2f> m_path;
    TimedLinearInterpolator<Vector2f> m_interpolator;
    int m_currIdx;
    float m_timePerStep;
};

class AnimColourMod : public Animation
{
public:
    AnimColourMod( Level* level, EntityRef entity, Colour startColour, Colour endColour, float seconds );

protected:
    void advanceSelf(std::uint32_t ticks) override;
    void onStart() override;

private:
    EntityRef m_entity;
    Colour m_startColour;
    Colour m_endColour;
};

// TODO Unfinished
class AnimLinearSpline : public Animation
{
public:
    AnimLinearSpline( Level* level, EntityRef entity, Vector2i target, Spline const& spline, float seconds );

protected:
    void advanceSelf(std::uint32_t ticks) override;

private:
    EntityRef m_entity;
    Vector2i m_target;
    Vector2f m_vector;
    Spline m_spline;
};
