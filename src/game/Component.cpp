#include <game/Component.h>

#include <utility>

ComponentId BaseComponent::m_globalId = 0;

Components::Render::Render(Sprite const &s)
: sprites({s}), current(0)
{
}

Components::Render::Render(std::vector<Sprite> const &ss)
: sprites(ss), current(0)
{

}

Components::TilePosition::TilePosition(Vector2i const &p)
: position(p)
{
}

Components::FixedState::FixedState(std::vector<std::string>const& states)
: states(states), current(0)
{
}

Components::Description::Description(std::string val)
: descriptions({std::move(val)}), current(0)
{
}

Components::Description::Description(std::vector<std::string> const& vals)
: descriptions(vals), current(0)
{
}
