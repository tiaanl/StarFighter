
#ifndef STAR_FIGHTER_COMPONENTS_POSITION_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_POSITION_COMPONENT_H_

#include "canvas/Math/Vec2.h"
#include "nucleus/Types.h"

struct PositionComponent {
  // Current position of the entity.
  ca::Vec2 pos{};

  // Current direction the entity is facing in degrees.  (0.f..360.f)
  F32 direction{0.f};

  // Size of the entity.
  F32 size{0.f};

  // Whether the entity is selectable or not.
  bool selectable{false};
};

#endif  // STAR_FIGHTER_COMPONENTS_POSITION_COMPONENT_H_
