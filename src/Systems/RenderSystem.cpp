
#include "StarFighter/Systems/RenderSystem.h"

#if 0

#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "canvas/Math/Transform.h"

#include "nucleus/MemoryDebug.h"

void RenderSystem::configure(ju::EntityManager& events) {
  LOG(Info) << "Configure RenderSystem";
}

void RenderSystem::update(ju::EntityManager& entities, ca::Canvas* canvas, const ca::Mat4& mat) {
  // Render all the entities.
  auto view = entities.allEntitiesWithComponent<PositionComponent, SpriteComponent>();
  for (auto& entity : view) {
    auto sprite = entity.getComponent<SpriteComponent>();

    if (!sprite->visible) {
      continue;
    }

    auto position = entity.getComponent<PositionComponent>();

    {
      ca::Mat4 newMat{mat};
      newMat *= ca::translate(position->pos.x, position->pos.y, 0.f);
      newMat *= ca::rotate(ca::degreesToRadians(position->direction), ca::Vec3{0.f, 0.f, 1.f});

      sprite->icon->render(canvas, newMat);
    }

    if (sprite->movementTarget) {
      auto movement = entity.getComponent<MovementComponent>();

      ca::Mat4 newMat{mat};
      newMat *= ca::translate(movement->targetPos.x, movement->targetPos.y, 0.f);
      newMat *= ca::rotate(ca::degreesToRadians(position->direction), ca::Vec3{0.f, 0.f, 1.f});

      sprite->movementTarget->render(canvas, newMat);
    }
  }
}

#endif  // 0
