
#ifndef STAR_FIGHTER_COMPONENTS_COMBAT_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_COMBAT_COMPONENT_H_

#include "StarFighter/Data.h"
#include "junctions/entity.h"
#include "nucleus/types.h"

struct CombatComponent {
    enum {
        // We are the player and we make our own decisions.
        ModePlayer,

        // We're just idle, looking for something to attack.
        ModeIdle,

        // We're actively chasing a target.
        ModeChaseTarget,
    } combatMode{ModeIdle};

    // Which faction does this entity belong to?
    FactionType faction{FactionFoe};

    // The entity that spawned us.
    ju::EntityId sourceEntityId{ju::kInvalidEntityId};

    // Whether the entity is busy firing or not.
    bool isFiring{false};

    // The time that should elapse before we can fire the next shot.
    F32 fireRate{0.f};

    // The distance we can fire.
    F32 fireRange{300.f};

    // Elapsed time since last fire triggered.
    F32 timeSinceLastFire{0.f};

    // Does the entity die when it collides with something?  This is common for ammo entities.
    bool diesOnCollision{false};

    // The amount of health that the entity has.
    F32 health{100.f};

    // The target we're chasing.
    ju::EntityId chaseTargetId{ju::kInvalidEntityId};
};

#endif  // STAR_FIGHTER_COMPONENTS_COMBAT_COMPONENT_H_
