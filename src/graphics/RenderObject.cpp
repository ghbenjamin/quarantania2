#include <graphics/RenderObject.h>

RenderObject::RenderObject()
    : handle(0),
      screenBounds{0, 0, 0, 0},
      verts{
        0.0f, 1.0f, 0, 0,
        1.0f, 0.0f, 0, 0,
        0.0f, 0.0f, 0, 0,
        
        0.0f, 1.0f, 0, 0,
        1.0f, 1.0f, 0, 0,
        1.0f, 0.0f, 0, 0
      },
      renderLayer(RenderLayer::Begin)
 {}




