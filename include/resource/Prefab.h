#pragma once

#include <variant>

#include <resource/Spritesheet.h>

namespace Prefab
{
    namespace Component
    {
        struct Render
        {
            int renderStates = -1;
            std::vector<SpritesheetKey> sprites;
        };

        struct State
        {

        };

        struct Collider
        {
            bool defaultState = false;
        };

        struct Container
        {

        };
    }

    struct Visitor
    {
        void operator()(Component::Render const& obj) const
        {

        };

        void operator()(Component::State const& obj) const
        {

        };

        void operator()(Component::Collider const& obj) const
        {

        };

        void operator()(Component::Container const& obj) const
        {

        };
    };

}

using PrefabVariant = std::variant
    <
        Prefab::Component::Render,
        Prefab::Component::State,
        Prefab::Component::Collider,
        Prefab::Component::Container
    >;

using PrefabList = std::vector<PrefabVariant>;