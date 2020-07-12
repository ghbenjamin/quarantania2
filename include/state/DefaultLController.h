#pragma once

#include <state/LevelController.h>

class DefaultLController : public LevelController
{

public:
    explicit DefaultLController(Level *level);
    ~DefaultLController() override = default;

    void update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:

    bool onMouseMove(IEventMouseMove evt) override;
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;
    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;
};