
#ifndef STAR_FIGHTER_COMPONENTS_ASTEROID_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_ASTEROID_COMPONENT_H_

#include "nucleus/types.h"

struct AsteroidComponent {
  // The amount of minerals contained inside this asteroid.
  F32 mineralCount{0.f};
};

#endif  // STAR_FIGHTER_COMPONENTS_ASTEROID_COMPONENT_H_
