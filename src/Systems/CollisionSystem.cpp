
#include "StarFighter/Systems/CollisionSystem.h"

#include "StarFighter/Components/AmmoComponent.h"
#include "StarFighter/Components/CollisionComponent.h"
#include "StarFighter/Components/CombatComponent.h"
#include "StarFighter/Components/PositionComponent.h"

void CollisionSystem::update(ju::EntityManager& entities) {
    // Go through each entity that is ammo and see it it hit something.
    for (auto& ammoEntity : entities.allEntitiesWithComponent<PositionComponent, CollisionComponent, AmmoComponent>()) {
        for (auto& combatEntity :
             entities.allEntitiesWithComponent<PositionComponent, CollisionComponent, CombatComponent>()) {
            // Did we hit something?
            if (isColliding(ammoEntity, combatEntity)) {
                auto ammo = ammoEntity.getComponent<AmmoComponent>();
                auto combat = combatEntity.getComponent<CombatComponent>();

                // No friendly fire.
                if (ammo->faction == combat->faction) {
                    continue;
                }

                LOG(Info) << "Ammo entity " << ammoEntity.getId() << " collided with ship entity "
                          << combatEntity.getId();

                // Do damage to the combat entity.
                if (ammo->damageAmount > 0.f) {
                    // We hurt the other entity's health.
                    combat->health -= ammo->damageAmount;

                    // If we killed the combat entity, then it must be removed.
                    if (combat->health <= 0) {
                        combatEntity.remove();
                    }
                }

                // We collided with something, so we die.
                ammoEntity.remove();
            }
        }
    }
}

bool CollisionSystem::isColliding(ju::Entity& entity1, ju::Entity& entity2) const {
    auto position1 = entity1.getComponent<PositionComponent>();
    auto collision1 = entity1.getComponent<CollisionComponent>();

    auto position2 = entity2.getComponent<PositionComponent>();
    auto collision2 = entity2.getComponent<CollisionComponent>();

    F32 collisionDistance = collision1->collisionRadius + collision2->collisionRadius;

    F32 distanceBetweenEntities = ca::length(position2->pos - position1->pos);

    return (distanceBetweenEntities < collisionDistance);
}
