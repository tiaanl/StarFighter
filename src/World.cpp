
#include "StarFighter/World.h"

#include <random>

#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "StarFighter/Systems/MovementSystem.h"
#include "StarFighter/Systems/RenderSystem.h"
#include "canvas/Math/Transform.h"
#include "nucleus/Streams/FileInputStream.h"

#include "nucleus/MemoryDebug.h"

World::World(ca::ResourceCache* resourceCache) : m_resourceCache(resourceCache), m_entities{}, m_systems{&m_entities} {}

World::~World() {}

bool World::create() {
  // Add the systems.
  m_systems.addSystem<MovementSystem>();
  m_systems.addSystem<RenderSystem>();

  // Load all our textures.
  m_mousePointerTexture = m_resourceCache->getTexture("MouseCursor.png");
  m_starFighterTexture = m_resourceCache->getTexture("StarFighter.png");
  m_enemyFighterTexture = m_resourceCache->getTexture("Enemy1.png");
  m_movementTargetTexture = m_resourceCache->getTexture("MovementTarget.png");
  m_asteroidTexture = m_resourceCache->getTexture("asteroid-1.png");
  m_bargeTexture = m_resourceCache->getTexture("mining-barge.png");
  m_hoverTexture = m_resourceCache->getTexture("hover.png");
  m_selectedTexture = m_resourceCache->getTexture("selected.png");

  if (!spawnStarFighter()) {
    return false;
  }

  m_hoverEntityId = spawnHoverEntity();
  // m_selectedEntityId = spawnSelectedEntity();

  if (!spawnMouseCursor()) {
    return false;
  }

  if (!spawnStarFighter()) {
    return false;
  }

  posessEntity(m_starFighterEntityId);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> intDist(-1000, 1000);
  std::uniform_real_distribution<> realDist(0., 360.);
#if 0
  for (auto i = 0; i < 20; ++i) {
    if (!spawnEnemyFighter(ca::Vec2{static_cast<F32>(intDist(gen)), static_cast<F32>(intDist(gen))})) {
      return false;
    }
  }
#endif  // 0

  for (auto i = 0; i < 20; ++i) {
    if (!spawnAsteroid(ca::Vec2{static_cast<F32>(intDist(gen)), static_cast<F32>(intDist(gen))},
                       static_cast<F32>(realDist(gen)))) {
      return false;
    }
  }

#if 0
  if (!spawnBarge(ca::Vec2{})) {
      return false;
  }
#endif  // 0

  return true;
}

void World::setViewportSize(const ca::Size<U32>& viewportSize) {
  m_viewportSize = viewportSize;
}

void World::update(ca::Canvas* canvas, const ca::Size<U32>& viewportSize, F32 adjustment) {
  // Update the camera animations.
  m_camera.update(adjustment);

  // Get the final version of the camera's model/view matrix.
  ca::Mat4 mat = m_camera.calculateMatrix(viewportSize);

  // Update the mouse position in the world.
  ca::Pos<F32> mousePos(static_cast<F32>(m_mousePosition.x), static_cast<F32>(m_mousePosition.y));
  m_mouseWorldPos = m_camera.unproject(m_viewportSize, mousePos);

  auto position = m_entities.getComponent<PositionComponent>(m_mousePointerEntityId);
  position->pos = ca::Vec2{m_mouseWorldPos.x, m_mouseWorldPos.y};

  if (m_posessedEntity.isSettingMovementTarget) {
    if (m_posessedEntity.id != ju::kInvalidEntityId) {
      auto movement = m_entities.getComponent<MovementComponent>(m_posessedEntity.id);
      movement->moveToPos(m_mouseWorldPos, 100.f);
    }
  }

  auto cursorSprite = m_entities.getComponent<SpriteComponent>(m_hoverEntityId);
  ju::EntityId underMouseEntityId = getEntityUnderMouse(m_mouseWorldPos);
  if (underMouseEntityId != ju::kInvalidEntityId) {
    auto cursorPosition = m_entities.getComponent<PositionComponent>(m_hoverEntityId);
    auto underPosition = m_entities.getComponent<PositionComponent>(underMouseEntityId);

    cursorPosition->pos = underPosition->pos;
    cursorSprite->visible = true;
  } else {
    cursorSprite->visible = false;
  }

  // Move each entity into next step position according to what we decided it wants to do.
  m_systems.update<MovementSystem>(adjustment);

  // Render all the entities.
  m_systems.update<RenderSystem>(canvas, mat);

  // Perform any updates on the entity manager.
  m_entities.update();
}

void World::onMouseMoved(const ca::MouseEvent& evt) {
  m_mousePosition = evt.pos;

  // If we're setting the camera position, then update the position.
  if (m_cameraMovement.isSettingPos) {
    auto delta = ca::Pos<F32>{static_cast<F32>(m_cameraMovement.startPos.x - evt.pos.x),
                              static_cast<F32>(evt.pos.y - m_cameraMovement.startPos.y)};

    m_camera.setPosition(m_camera.getPosition() + delta);

    m_cameraMovement.startPos = evt.pos;
  }
}

bool World::onMousePressed(const ca::MouseEvent& evt) {
  // If we left click and we're hovering over an entity, then we try to select it.
  ju::EntityId entityUnderMouseId = getEntityUnderMouse(m_mouseWorldPos);
  if (evt.button == ca::MouseEvent::Button::Left) {
    m_posessedEntity.isSettingMovementTarget = true;
    return true;
  }

  if (evt.button == ca::MouseEvent::Button::Right) {
    // This will be set to true as long as the mouse button is pressed down.
    m_cameraMovement.isSettingPos = true;
    m_cameraMovement.startPos = evt.pos;

    // We want to receive an onMouseRelease event.
    return true;
  }

  return false;
}

void World::onMouseReleased(const ca::MouseEvent& evt) {
  if (m_posessedEntity.isSettingMovementTarget && evt.button == ca::MouseEvent::Left) {
    m_posessedEntity.isSettingMovementTarget = false;
  }

  if (m_cameraMovement.isSettingPos && evt.button == ca::MouseEvent::Right) {
    m_cameraMovement.isSettingPos = false;
  }
}

void World::onMouseWheel(const ca::MouseWheelEvent& evt) {
  m_camera.zoomTo(m_camera.getZoom() + static_cast<F32>(evt.wheelOffset.y) * 0.1f);
}

void World::onKeyPressed(const ca::KeyEvent& evt) {}

void World::onKeyReleased(const ca::KeyEvent& evt) {}

nu::ScopedPtr<ca::Texture> World::createTexture(const nu::FilePath& filename) {
  nu::FileInputStream stream(filename);

  ca::Image image;
  if (!image.loadFromStream(&stream)) {
    LOG(Warning) << "Could not load ship image.";
    return nullptr;
  }

  auto texture = nu::makeScopedPtr<ca::Texture>();
  texture->createFromImage(image);

  return texture;
}

bool World::spawnStarFighter() {
  m_starFighterEntityId = m_entities.createEntity();
  ju::Entity* entity = m_entities.getEntity(m_starFighterEntityId);

  // PositionComponent
  auto position = entity->addComponent<PositionComponent>();
  position->selectable = true;
  position->size = 32.f;

  // MovementComponent
  auto movement = entity->addComponent<MovementComponent>();
  movement->currentSpeed = 0.f;
  movement->maxSpeed = 5.f;

  // SpriteComponent
  auto sprite = entity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_starFighterTexture);

  return true;
}

bool World::spawnMouseCursor() {
  m_mousePointerEntityId = m_entities.createEntity();
  auto mousePointerEntity = m_entities.getEntity(m_mousePointerEntityId);
  mousePointerEntity->addComponent<PositionComponent>();

  auto sprite = mousePointerEntity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_mousePointerTexture);

  return true;
}

bool World::spawnEnemyFighter(const ca::Vec2& pos) {
  auto entityId = m_entities.createEntity();
  ju::Entity* entity = m_entities.getEntity(entityId);

  // PositionComponent
  auto position = entity->addComponent<PositionComponent>();
  position->pos = pos;

  // MovementComponent
  auto movement = entity->addComponent<MovementComponent>();
  movement->maxSpeed = 2.5f;

  // SpriteComponent
  auto sprite = entity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_enemyFighterTexture);
  // sprite->movementTarget = nu::MakeScopedPtr<ca::Sprite>(m_movementTargetTexture.get());

  return true;
}

bool World::spawnAsteroid(const ca::Vec2& pos, F32 direction) {
  auto entityId = m_entities.createEntity();
  ju::Entity* entity = m_entities.getEntity(entityId);

  // PositionComponent
  auto position = entity->addComponent<PositionComponent>();
  position->pos = pos;
  position->direction = direction;
  position->size = 32.f;
  position->selectable = true;

  // SpriteComponent
  auto sprite = entity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_asteroidTexture);

  return true;
}

bool World::spawnBarge(const ca::Vec2& pos) {
  auto entityId = m_entities.createEntity();
  ju::Entity* entity = m_entities.getEntity(entityId);

  // PositionComponent
  auto position = entity->addComponent<PositionComponent>();
  position->pos = pos;

  // MovementComponent
  auto movement = entity->addComponent<MovementComponent>();
  movement->maxSpeed = 2.5f;

  // SpriteComponent
  auto sprite = entity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_bargeTexture);

  return true;
}

ju::EntityId World::spawnHoverEntity() {
  ju::EntityId id = m_entities.createEntity();
  auto entity = m_entities.getEntity(id);

  auto position = entity->addComponent<PositionComponent>();
  position->pos = ca::Vec2{0.f, 0.f};

  auto sprite = entity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_hoverTexture);

  return id;
}

ju::EntityId World::spawnSelectedEntity() {
  ju::EntityId id = m_entities.createEntity();
  auto entity = m_entities.getEntity(id);

  auto position = entity->addComponent<PositionComponent>();
  position->pos = ca::Vec2{0.f, 0.f};

  auto sprite = entity->addComponent<SpriteComponent>();
  sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_selectedTexture);

  return id;
}

void World::posessEntity(ju::EntityId id) {
  // If we previously posessed an entity, then make sure we can select it again.
  if (m_posessedEntity.id != ju::kInvalidEntityId) {
    auto position = m_entities.getComponent<PositionComponent>(m_posessedEntity.id);
    position->selectable = true;
  }

  m_posessedEntity.id = id;

  // The entity we're posessing now can not be selected.
  if (id != ju::kInvalidEntityId) {
    auto position = m_entities.getComponent<PositionComponent>(m_posessedEntity.id);
    position->selectable = false;
  }
}

ju::EntityId World::getEntityUnderMouse(const ca::Vec2& mouseWorldPos) {
  auto view = m_entities.allEntitiesWithComponent<PositionComponent>();
  for (auto& entity : view) {
    auto position = entity.getComponent<PositionComponent>();

    if (!position->selectable) {
      continue;
    }

    F32 xd = mouseWorldPos.x - position->pos.x;
    F32 yd = mouseWorldPos.y - position->pos.y;

    F32 left = ((xd * xd) + (yd * yd));
    F32 right = (position->size * position->size);

    bool inRange = left <= right;

    // LOG(Info) << "Checking entity: " << entity.getId() << " (" << left << " <= " << right << ")";

    if (inRange) {
      return entity.getId();
    }
  }

  return ju::kInvalidEntityId;
}
