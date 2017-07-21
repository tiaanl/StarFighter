
#ifndef STAR_FIGHTER_SYSTEMS_MOVEMENT_SYSTEM_H_
#define STAR_FIGHTER_SYSTEMS_MOVEMENT_SYSTEM_H_

#include "canvas/math/mat4.h"
#include "canvas/rendering/canvas.h"
#include "junctions/entity_manager.h"

class MovementSystem {
public:
    void configure(ju::EntityManager& events);
    void update(ju::EntityManager& entities, F32 adjustment);

private:
    void handleMoveForward(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment);
    void handleMoveToPosition(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment);
    void handleFollowEntity(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment);
};

#endif  // STAR_FIGHTER_SYSTEMS_MOVEMENT_SYSTEM_H_
