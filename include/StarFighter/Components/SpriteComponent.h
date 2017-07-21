
#ifndef START_FIGHTER_COMPONENTS_SPRITE_COMPONENT_H_
#define START_FIGHTER_COMPONENTS_SPRITE_COMPONENT_H_

#include "canvas/Primitives/Sprite.h"
#include "nucleus/memory/ScopedPtr.h"

struct SpriteComponent {
    nu::ScopedPtr<ca::Sprite> icon;
    nu::ScopedPtr<ca::Sprite> movementTarget;
};

#endif  // START_FIGHTER_COMPONENTS_SPRITE_COMPONENT_H_
