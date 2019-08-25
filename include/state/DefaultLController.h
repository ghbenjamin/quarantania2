#pragma once

#include <state/LevelController.h>

class DefaultLController : public LevelController
{

public:
    explicit DefaultLController(Level *level);

private:

    void doMovePlayer(SDL_Keycode kcode);

    bool onMouseMove(IEventMouseMove evt) override;
    bool onMouseDown(IEventMouseDown evt) override;
    bool onKeyDown(IEventKeyPress evt) override;

    void onHoveredTileChange(Vector2i prev, Vector2i curr) override;
};