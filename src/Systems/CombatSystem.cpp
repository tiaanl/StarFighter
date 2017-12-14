
#include "StarFighter/Systems/CombatSystem.h"

#include "StarFighter/Components/AmmoComponent.h"
#include "StarFighter/Components/CollisionComponent.h"
#include "StarFighter/Components/CombatComponent.h"
#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "canvas/Math/Transform.h"

#include "nucleus/MemoryDebug.h"

CombatSystem::CombatSystem(nu::ScopedPtr<ca::Texture> bulletTexture) : m_bulletTexture(nu::move(bulletTexture)) {}

void CombatSystem::configure(ju::EntityManager& entities) {
  entities.subscribe<InputEvent, CombatSystem>(this);
}

void CombatSystem::update(ju::EntityManager& entities, F32 adjustment) {
  for (auto& entity : entities.allEntitiesWithComponent<CombatComponent>()) {
    auto combat = entity.getComponent<CombatComponent>();

    switch (combat->combatMode) {
      case CombatComponent::ModePlayer:
        // We don't do any decision making for the player.
        break;

      case CombatComponent::ModeIdle:
        handleModeIdle(entities, entity, adjustment);
        break;

      case CombatComponent::ModeChaseTarget:
        handleModeChaseTarget(entities, entity, adjustment);
        break;

      default:
        NOTREACHED() << "Invalid combat mode.";
    }

    if (combat->isFiring) {
      if (combat->timeSinceLastFire > combat->fireRate) {
        combat->timeSinceLastFire = 0.f;

        spawnBullet(entities, entity);
      } else {
        F32 timeElapsed = 1.f / 60.f * adjustment;
        combat->timeSinceLastFire += timeElapsed;
      }
    }
  }
}

void CombatSystem::receive(ju::EntityManager& entities, const InputEvent& evt) {
  // Get the entity that is receiving this event.
  ju::Entity* sourceEntity = entities.getEntity(evt.id);
  if (!sourceEntity) {
    return;
  }

  auto combat = sourceEntity->getComponent<CombatComponent>();
  if (!combat) {
    return;
  }

  switch (evt.action) {
    case InputEvent::ActionStartFiring:
      combat->isFiring = true;

      // This forces the first shot to be fired immediately.
      combat->timeSinceLastFire = combat->fireRate;
      break;

    case InputEvent::ActionStopFiring:
      combat->isFiring = false;
      break;

    default:
      NOTREACHED() << "Invalid action type";
  }
}

void CombatSystem::handleModeIdle(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) const {
  auto position = entity.getComponent<PositionComponent>();
  auto combat = entity.getComponent<CombatComponent>();

  F32 closestDistance = 300.f;
  ju::EntityId closestTargetId = ju::kInvalidEntityId;

  // Try to find something to attack.
  for (auto& target : entities.allEntitiesWithComponent<PositionComponent, CombatComponent>()) {
    auto targetCombat = target.getComponent<CombatComponent>();
    if (targetCombat->faction == combat->faction) {
      continue;
    }

    // Calculate the distance to the target.
    auto targetPosition = target.getComponent<PositionComponent>();
    F32 distanceToTarget = ca::length(position->pos - targetPosition->pos);

    if (distanceToTarget < closestDistance) {
      closestDistance = distanceToTarget;
      closestTargetId = target.getId();
    }
  }

  if (closestTargetId != ju::kInvalidEntityId) {
    // LOG(Info) << "Entity " << entity.getId() << " found target " << closestTargetId << " at " << closestDistance
    // << "...chasing";

    combat->combatMode = CombatComponent::ModeChaseTarget;
    combat->chaseTargetId = closestTargetId;
  }
}

void CombatSystem::handleModeChaseTarget(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) const {
  auto position = entity.getComponent<PositionComponent>();
  auto combat = entity.getComponent<CombatComponent>();

  // If we don't have a target any more, then go back to idle.
  if (combat->chaseTargetId == ju::kInvalidEntityId) {
    combat->combatMode = CombatComponent::ModeIdle;
    return;
  }

  ju::Entity* targetEntity = entities.getEntity(combat->chaseTargetId);

  // If we can't find the entity, we just go back to idle mode.
  if (!targetEntity) {
    combat->combatMode = CombatComponent::ModeIdle;
    combat->chaseTargetId = ju::kInvalidEntityId;

    return;
  }

  // LOG(Info) << "Entity " << entity.getId() << " is chasing entity " << combat->chaseTargetId;

  // How far are we from the chase target?
  auto targetPosition = entities.getComponent<PositionComponent>(combat->chaseTargetId);
  F32 distanceToTarget = ca::length(position->pos - targetPosition->pos);

  F32 minimumDistanceToGetInRange = std::max(0.f, distanceToTarget - (combat->fireRange * 0.8f));

  F32 angleToTarget = 90.f - ca::radiansToDegrees(ca::angleBetweenInRadians(position->pos, targetPosition->pos));

  // If we aren't in range, then set a target to get into range.
  if (minimumDistanceToGetInRange > 0.f) {
    auto movement = entity.getComponent<MovementComponent>();

    F32 inRadians = ca::degreesToRadians(angleToTarget);
    ca::Vec2 diff{std::cos(inRadians) * minimumDistanceToGetInRange, std::sin(inRadians) * minimumDistanceToGetInRange};
    ca::Vec2 newPos = position->pos + diff;

    movement->moveToPos(newPos, movement->maxSpeed);

    // LOG(Info) << "Entity " << entity.getId() << " is setting intercept position for " << newPos;
  } else {
    // Keep tracking the target.
    position->direction = angleToTarget + 90.f;
  }

  combat->isFiring = distanceToTarget < combat->fireRange;
}

void CombatSystem::spawnBullet(ju::EntityManager& entities, ju::Entity& source) const {
  auto sourcePosition = source.getComponent<PositionComponent>();
  auto sourceCombat = source.getComponent<CombatComponent>();

  ju::EntityId bulletId = entities.createEntity();
  auto bullet = entities.getEntity(bulletId);

  // PositionComponent
  auto position = bullet->addComponent<PositionComponent>();
  position->pos = sourcePosition->pos;
  position->direction = sourcePosition->direction;

  // CollisionComponent
  auto collision = bullet->addComponent<CollisionComponent>();
  collision->collisionRadius = 10.f;

  auto ammo = bullet->addComponent<AmmoComponent>();
  ammo->damageAmount = 10.f;
  ammo->faction = sourceCombat->faction;

  // SpriteComponent
  auto sprite = bullet->addComponent<SpriteComponent>();
  sprite->icon.reset(new ca::Sprite(m_bulletTexture.get()));
}
