#include <graphics/RenderObject.h>

RenderObject::RenderObject()
    : texture(nullptr),
      sourceRect{0, 0, 0, 0},
      targetRect{0, 0, 0, 0},
      clipRect{0, 0, 0, 0},
      renderLayer(RenderLayer::Begin)
{
}

RenderObject::RenderObject(SDL_Texture *texture, SDL_Rect src, SDL_Rect dst)
    : texture(texture),
      sourceRect(src),
      targetRect(dst),
      clipRect{0, 0, 0, 0},
      renderLayer(RenderLayer::Begin)
{
}





