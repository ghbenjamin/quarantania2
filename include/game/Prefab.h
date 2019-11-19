#pragma once

#include <variant>

#include <resource/Spritesheet.h>
#include <utils/Random.h>
#include <game/Entity.h>

class Level;

namespace PrefabComponent
{

struct Render
{
    std::vector<SpritesheetKey> sprites;
    std::vector<int> spriteBreakpoints;
};

struct State
{
    std::vector<std::string> states;
};

struct Collider
{
    bool blocksMovement = false;
    bool blocksLight = true;
};

struct Container
{

};

struct Description
{
    std::vector<std::string> descriptions;
};
}

class PrefabVisitor
{
public:
    explicit PrefabVisitor(Level* level, EntityRef ref, RandomGenerator* rg);
    ~PrefabVisitor() = default;

    void operator()(PrefabComponent::Render const& obj) const;
    void operator()(PrefabComponent::State const& obj) const;
    void operator()(PrefabComponent::Collider const& obj) const;
    void operator()(PrefabComponent::Container const& obj) const;
    void operator()(PrefabComponent::Description const& obj) const;

private:
    EntityRef m_ref;
    Level* m_level;
    RandomGenerator* m_rg;
};

using PrefabVariant = std::variant
        <
                PrefabComponent::Render,
                PrefabComponent::State,
                PrefabComponent::Collider,
                PrefabComponent::Container,
                PrefabComponent::Description
        >;

using PrefabList = std::vector<PrefabVariant>;