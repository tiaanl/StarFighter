
#ifndef STAR_FIGHTER_SYSTEMS_MINING_SYSTEM_H_
#define STAR_FIGHTER_SYSTEMS_MINING_SYSTEM_H_

#include "canvas/Math/Vec2.h"
#include "junctions/EntityManager.h"
#include "nucleus/Types.h"

class MiningSystem {
public:
  void update(ju::EntityManager& entities, F32 adjustment);

private:
  ju::EntityId findClosestAsteroid(const ca::Vec2& origin, ju::EntityManager& entities);

  /*
   * public:
  void configure(ju::EntityManager& events);
  void update(ju::EntityManager& entities, F32 adjustment);

private:
  void handleMoveForward(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment);
  void handleMoveToPosition(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment);
  void handleFollowEntity(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment);

   */
};

#endif  // STAR_FIGHTER_SYSTEMS_MINING_SYSTEM_H_
