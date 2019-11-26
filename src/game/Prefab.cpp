#include <game/Prefab.h>
#include <game/Level.h>
#include <resource/ResourceManager.h>
#include <components/All.h>

PrefabVisitor::PrefabVisitor(Level* level, EntityRef ref, RandomGenerator* rg)
        : m_ref(ref), m_level(level), m_rg(rg) {}

void PrefabVisitor::operator()(PrefabComponent::Render const& obj) const
{
    // TODO: something more sophisticated than randomly picking

    auto begin_it = obj.sprites.begin();
    auto end_it = obj.sprites.begin();
    std::advance(end_it, obj.spriteBreakpoints[0]);

    auto it = randomElement(begin_it, end_it, *m_rg);
    auto sprite = ResourceManager::get().getSprite(*it);

    m_level->addComponent<Components::Render>(m_ref, sprite);
}

void PrefabVisitor::operator()(PrefabComponent::Collider const& obj) const
{
    m_level->addComponent<Components::Collider>(m_ref, obj.blocksLight, obj.blocksMovement);
}

void PrefabVisitor::operator()(PrefabComponent::Container const& obj) const
{
}

void PrefabVisitor::operator()(PrefabComponent::Description const& obj) const
{
    m_level->addComponent<Components::Description>(m_ref, obj.descriptions);
}