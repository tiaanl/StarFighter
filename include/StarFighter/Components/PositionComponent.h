
#ifndef STAR_FIGHTER_COMPONENTS_POSITION_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_POSITION_COMPONENT_H_

#include "canvas/math/vec2.h"
#include "nucleus/types.h"

struct PositionComponent {
    // Current position of the entity.
    ca::Vec2 pos;

    // Current rotation of the entity.
    F32 rot;

    PositionComponent(const ca::Vec2& pos = ca::Vec2{}, F32 rot = 0.f) : pos(pos), rot(rot) {}
};

#endif  // STAR_FIGHTER_COMPONENTS_POSITION_COMPONENT_H_
