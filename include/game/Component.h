#pragma once

#include <vector>
#include <string>

using ComponentId = std::size_t;

struct BaseComponent
{
public:
    BaseComponent() = default;
    virtual ~BaseComponent() = default;

protected:
    static ComponentId m_globalId;
};

template <typename ET>
struct Component : public BaseComponent
{
    Component() = default;
    ~Component() override = default;

    static ComponentId id()
    {
        static ComponentId m_id = m_globalId++;
        return m_id;
    }
};