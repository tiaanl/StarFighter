
#ifndef STAR_FIGHTER_COMPONENTS_TARGET_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_TARGET_COMPONENT_H_

#include "junctions/Entity.h"

struct TargetComponent {
  // The current target of the entity.
  ju::EntityId currentTarget{ju::kInvalidEntityId};
};

#endif  // STAR_FIGHTER_COMPONENTS_TARGET_COMPONENT_H_
