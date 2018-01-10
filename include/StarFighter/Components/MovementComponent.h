
#ifndef STAR_FIGHTER_COMPONENTS_MOVEMENT_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_MOVEMENT_COMPONENT_H_

#if 0
#include "canvas/Math/Vec2.h"

struct MovementComponent {
  enum {
    MoveForward,
    MoveToPosition,
    FollowEntity,
  } moveStrategy{MoveForward};

  // The target position the entity is moving towards.
  ca::Vec2 targetPos{0.f, 0.f};

  // The target entity ID we're moving towards.
  ju::EntityId targetEntityId{ju::kInvalidEntityId};

  // If we're not doing targeted movement, then we just move in a direction.
  F32 direction{0.f};

  // The current speed the entity is moving at.
  F32 currentSpeed{0.f};

  // The maximum speed the entity can move at.
  F32 maxSpeed{0.f};

  // The distance this entity has travelled.
  F32 distanceTravelled{0.f};

  // When the entity reaches this distance, it dies.  If it is 0, then we ignore the max distance.
  F32 maxDistanceBeforeDie{0.f};

  void moveToPos(const ca::Vec2& pos, F32 speed) {
    moveStrategy = MoveToPosition;
    targetPos = pos;
    direction = 0.f;
    currentSpeed = speed;
  }

  void setBulletMovement(F32 direction, F32 speed) {
    moveStrategy = MoveForward;
    this->direction = direction;
    currentSpeed = speed;
  }

  void followEntity(ju::EntityId entityId, F32 speed) {
    moveStrategy = FollowEntity;
    targetEntityId = entityId;
    currentSpeed = speed;
  }
};

#endif  // 0

#endif  // STAR_FIGHTER_COMPONENTS_MOVEMENT_COMPONENT_H_
