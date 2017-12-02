
#include "StarFighter/Systems/MovementSystem.h"

#include "StarFighter/Components/AmmoComponent.h"
#include "StarFighter/Components/CombatComponent.h"
#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "canvas/math/transform.h"

#include "nucleus/MemoryDebug.h"

void MovementSystem::configure(ju::EntityManager& events) {}

void MovementSystem::update(ju::EntityManager& entities, F32 adjustment) {
  // Move all ships.
  for (auto& entity : entities.allEntitiesWithComponent<PositionComponent, MovementComponent>()) {
    auto movement = entity.getComponent<MovementComponent>();

    switch (movement->moveStrategy) {
      case MovementComponent::MoveForward:
        handleMoveForward(entities, entity, adjustment);
        break;

      case MovementComponent::MoveToPosition:
        handleMoveToPosition(entities, entity, adjustment);
        break;

      case MovementComponent::FollowEntity:
        handleFollowEntity(entities, entity, adjustment);
        break;
    }

    if (movement->maxDistanceBeforeDie > 0.f && movement->distanceTravelled > movement->maxDistanceBeforeDie) {
      entity.remove();
    }
  }

  // Move all ammo.
  for (auto& ammoEntity : entities.allEntitiesWithComponent<PositionComponent, AmmoComponent>()) {
    auto position = ammoEntity.getComponent<PositionComponent>();
    auto ammo = ammoEntity.getComponent<AmmoComponent>();

    F32 dirInRadians = ca::degreesToRadians(position->direction - 90.f);

    // Calculate the next movement step.
    ca::Vec2 v{std::cos(dirInRadians) * ammo->velocity * adjustment,
               std::sin(dirInRadians) * ammo->velocity * adjustment};

    position->pos += v;
  }
}

void MovementSystem::handleMoveForward(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) {
  auto position = entity.getComponent<PositionComponent>();
  auto movement = entity.getComponent<MovementComponent>();

  auto inRadians = ca::degreesToRadians(movement->direction - 90.f);
  ca::Vec2 diff{std::cos(inRadians) * movement->currentSpeed * adjustment,
                std::sin(inRadians) * movement->currentSpeed * adjustment};

  // Update the distance travelled.
  movement->distanceTravelled += ca::length(diff);

  position->pos += diff;
}

void MovementSystem::handleMoveToPosition(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) {
  auto position = entity.getComponent<PositionComponent>();
  auto movement = entity.getComponent<MovementComponent>();

  // If we are at the target position, then we do nothing.
  if (ca::closeTo(position->pos, movement->targetPos)) {
    return;
  }

  // We have somewhere to move, so go full speed.
  movement->currentSpeed = movement->maxSpeed;

  // Calculate the vector the entity should be moving at.
  ca::Vec2 dir = movement->targetPos - ca::Vec2{position->pos.x, position->pos.y};

  // Adjust the distance travelled.
  movement->distanceTravelled += ca::length(dir);

  // Set the rotation towards the target.
  position->direction = 180.f - ca::radiansToDegrees(ca::angleBetweenInRadians(position->pos, movement->targetPos));

  // If the distance to the target is less that the max speed, then we just snap to the target position.
  // Otherwise we move towards the target at the max speed.
  if (ca::length(dir) < movement->maxSpeed) {
    movement->currentSpeed = 0.f;
    position->pos = movement->targetPos;
  } else {
    position->pos += ca::normalize(dir) * movement->maxSpeed * adjustment;
  }
}

void MovementSystem::handleFollowEntity(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) {
  auto movement = entity.getComponent<MovementComponent>();
  auto targetEntityPos = entities.getComponent<PositionComponent>(movement->targetEntityId);
  movement->targetPos = targetEntityPos->pos;

  handleMoveToPosition(entities, entity, adjustment);
}
