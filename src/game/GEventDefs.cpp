#include <game/GEventDefs.h>

GEvents::EntityMove::EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos)
    : ent(ent), oldPos(oldPos), newPos(newPos) {}

GEvents::EntityReady::EntityReady(EntityRef ent)
    : ent(ent) {}

GEvents::EntityOpenClose::EntityOpenClose(EntityRef ent, bool isOpen)
    : ent(ent), isOpen(isOpen) {}

