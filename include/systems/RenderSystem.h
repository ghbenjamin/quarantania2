#pragma once

#include <systems/System.h>

class RenderSystem : public System
{
public:
    explicit RenderSystem(Level *parent);
    ~RenderSystem() override = default;

    void update(uint32_t ticks, RenderInterface &rInter) override;
};