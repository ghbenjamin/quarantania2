#pragma once

#include <systems/System.h>

namespace Systems
{

class Render : public System
{
public:
    explicit Render(Level *parent);
    ~Render() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;
};

}