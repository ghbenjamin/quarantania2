#include <components/Render.h>

Components::Render::Render(Sprite const &s)
    : sprites({s}), current(0)
{
}

Components::Render::Render(std::vector<Sprite> const &ss)
    : sprites(ss), current(0)
{

}
