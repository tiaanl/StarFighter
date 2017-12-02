
#include "StarFighter/Systems/MiningSystem.h"

#include "StarFighter/Components/AsteroidComponent.h"
#include "StarFighter/Components/MinerComponent.h"
#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/TargetComponent.h"

#include "nucleus/MemoryDebug.h"

void MiningSystem::update(ju::EntityManager& entities, F32 adjustment) {
  for (auto& entity :
       entities.allEntitiesWithComponent<PositionComponent, MovementComponent, MinerComponent, TargetComponent>()) {
    auto position = entity.getComponent<PositionComponent>();
    ju::EntityId closestAsteroid = findClosestAsteroid(position->pos, entities);

    if (closestAsteroid == ju::kInvalidEntityId) {
      continue;
    }

    auto target = entity.getComponent<TargetComponent>();
    if (target->currentTarget != ju::kInvalidEntityId) {
      continue;
    }

    target->currentTarget = closestAsteroid;

    auto targetPosition = entities.getComponent<PositionComponent>(closestAsteroid);

    LOG(Info) << "Barge moving to target";
    auto movement = entity.getComponent<MovementComponent>();
    movement->moveToPos(targetPosition->pos, 5.f);
  }
}

ju::EntityId MiningSystem::findClosestAsteroid(const ca::Vec2& origin, ju::EntityManager& entities) {
  F32 closestDistance = 1500.f;
  ju::EntityId closestEntity = ju::kInvalidEntityId;

  for (auto& target : entities.allEntitiesWithComponent<PositionComponent, AsteroidComponent>()) {
    // Calculate the distance to the target.
    auto targetPosition = target.getComponent<PositionComponent>();
    F32 distanceToTarget = ca::length(origin - targetPosition->pos);

    if (distanceToTarget < closestDistance) {
      closestDistance = distanceToTarget;
      closestEntity = target.getId();
    }
  }

  return closestEntity;
}
