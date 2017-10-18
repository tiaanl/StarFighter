
#ifndef STAR_FIGHTER_COMPONENTS_AMMO_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_AMMO_COMPONENT_H_

#include "StarFighter/Data.h"
#include "nucleus/types.h"

struct AmmoComponent {
  // The faction that fired this piece of ammo.
  FactionType faction{FactionFoe};

  // The amount of damage that this entity causes when colliding with another entity.
  F32 damageAmount{0.f};

  // The speed we are traveling at.
  F32 velocity{10.f};
};

#endif  // STAR_FIGHTER_COMPONENTS_AMMO_COMPONENT_H_
