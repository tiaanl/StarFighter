
#ifndef STAR_FIGHTER_SYSTEMS_COLLISION_SYSTEM_H_
#define STAR_FIGHTER_SYSTEMS_COLLISION_SYSTEM_H_

#include "junctions/EntityManager.h"

class CollisionSystem {
public:
  void update(ju::EntityManager& entities);

private:
  bool isColliding(ju::Entity& entity1, ju::Entity& entity2) const;
};

#endif  // STAR_FIGHTER_SYSTEMS_COLLISION_SYSTEM_H_
