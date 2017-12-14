
#include "StarFighter/World.h"

#include <StarFighter/Systems/MiningSystem.h>
#include <random>

#include "StarFighter/Components/AsteroidComponent.h"
#include "StarFighter/Components/CollisionComponent.h"
#include "StarFighter/Components/CombatComponent.h"
#include "StarFighter/Components/MinerComponent.h"
#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "StarFighter/Components/TargetComponent.h"
#include "StarFighter/Systems/CollisionSystem.h"
#include "StarFighter/Systems/CombatSystem.h"
#include "StarFighter/Systems/MovementSystem.h"
#include "StarFighter/Systems/RenderSystem.h"
#include "canvas/Math/Transform.h"
#include "nucleus/Streams/FileInputStream.h"

#include "nucleus/MemoryDebug.h"

World::World() : m_entities{}, m_systems{&m_entities} {}

World::~World() {}

bool World::create() {
#if OS(WIN32)
  nu::FilePath rootPath {FILE_PATH_LITERAL("C:\Code\StarFighter\assets\")};
#elif OS(MACOSX)
  nu::FilePath rootPath{FILE_PATH_LITERAL("/Users/tiaan.louw/Code/github.com/tiaanl/StarFighter/assets")};
#endif

  // Add the systems.
  m_systems.addSystem<CombatSystem>(createTexture(rootPath.append("Bullet.png")));
  m_systems.addSystem<MiningSystem>();
  m_systems.addSystem<MovementSystem>();
  m_systems.addSystem<CollisionSystem>();
  m_systems.addSystem<RenderSystem>();

  // Load all our textures.
  m_mousePointerTexture = createTexture(rootPath.append(FILE_PATH_LITERAL("MouseCursor.png")));
  m_starFighterTexture = createTexture(rootPath.append(FILE_PATH_LITERAL("StarFighter.png")));
  m_enemyFighterTexture = createTexture(rootPath.append(FILE_PATH_LITERAL("Enemy1.png")));
  m_movementTargetTexture = createTexture(rootPath.append(FILE_PATH_LITERAL("MovementTarget.png")));
  m_asteroidTexture = createTexture(rootPath.append(FILE_PATH_LITERAL("asteroid-1.png")));
  m_bargeTexture = createTexture(rootPath.append(FILE_PATH_LITERAL("mining-barge.png")));

  if (!spawnStarFighter()) {
      return false;
  }

  posessEntity(m_starFighterEntityId);

  if (!spawnMouseCursor()) {
      return false;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> intDist(-1000, 1000);
  std::uniform_real_distribution<> realDist(0., 360.);

  //    for (auto i = 0; i < 20; ++i) {
  //        if (!spawnEnemyFighter(ca::Vec2{static_cast<F32>(intDist(gen)), static_cast<F32>(intDist(gen))})) {
  //            return false;
  //        }
  //    }

  for (auto i = 0; i < 20; ++i) {
      if (!spawnAsteroid(ca::Vec2{static_cast<F32>(intDist(gen)), static_cast<F32>(intDist(gen))},
                         static_cast<F32>(realDist(gen)))) {
        return false;
      }
  }

  if (!spawnBarge(ca::Vec2{})) {
      return false;
  }

  return true;
  }

  void World::setViewportSize(const ca::Size<U32>& viewportSize) {
    m_viewportSize = viewportSize;
  }

  void World::update(ca::Canvas * canvas, const ca::Size<U32>& viewportSize, F32 adjustment) {
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

    // Decide what each entity wants to do.
    m_systems.update<CombatSystem>(adjustment);

    // Let the mining system figure out who mines what.
    m_systems.update<MiningSystem>(adjustment);

    // Move each entity into next step position according to what we decided it wants to do.
    m_systems.update<MovementSystem>(adjustment);

    // Check collision between entities that just moved.
    m_systems.update<CollisionSystem>();

    // Render all the entities.
    m_systems.update<RenderSystem>(canvas, mat);

    // Perform any updates on the entity manager.
    m_entities.update();
  }

  void World::onMouseMoved(const ca::MouseEvent& evt) {
    m_mousePosition = evt.pos;

    // If we're setting the camera position, then update the position.
    if (m_settingCameraPos) {
      auto delta = ca::Pos<F32>{static_cast<F32>(m_cameraPosStartPos.x - evt.pos.x),
                                static_cast<F32>(evt.pos.y - m_cameraPosStartPos.y)};

      m_camera.setPosition(m_camera.getPosition() + delta);

      m_cameraPosStartPos = evt.pos;
    }
  }

  bool World::onMousePressed(const ca::MouseEvent& evt) {
    if (evt.button == ca::MouseEvent::Button::Left) {
      m_posessedEntity.isSettingMovementTarget = true;
      return true;
    }

    if (evt.button == ca::MouseEvent::Button::Right) {
      // This will be set to true as long as the mouse button is pressed down.
      m_settingCameraPos = true;
      m_cameraPosStartPos = evt.pos;

      // We want to receive an onMouseRelease event.
      return true;
    }

    return false;
  }

  void World::onMouseReleased(const ca::MouseEvent& evt) {
    if (m_posessedEntity.isSettingMovementTarget && evt.button == ca::MouseEvent::Left) {
      m_posessedEntity.isSettingMovementTarget = false;
    }

    if (m_settingCameraPos && evt.button == ca::MouseEvent::Right) {
      m_settingCameraPos = false;
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
    entity->addComponent<PositionComponent>();

    // MovementComponent
    auto movement = entity->addComponent<MovementComponent>();
    movement->currentSpeed = 0.f;
    movement->maxSpeed = 5.f;

    // CollisionComponent
    auto collision = entity->addComponent<CollisionComponent>();
    collision->collisionRadius = 32.f;

    // SpriteComponent
    auto sprite = entity->addComponent<SpriteComponent>();
    sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_starFighterTexture.get());

    // CombatComponent
    auto combat = entity->addComponent<CombatComponent>();
    combat->combatMode = CombatComponent::ModePlayer;
    combat->faction = FactionFriend;
    combat->fireRate = 0.1f;
    combat->fireRange = 1000.f;
    combat->diesOnCollision = false;
    combat->health = 10000.f;

    return true;
  }

  bool World::spawnMouseCursor() {
    m_mousePointerEntityId = m_entities.createEntity();
    auto mousePointerEntity = m_entities.getEntity(m_mousePointerEntityId);
    mousePointerEntity->addComponent<PositionComponent>();

    auto sprite = mousePointerEntity->addComponent<SpriteComponent>();
    sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_mousePointerTexture.get());

    return true;
  }

  bool World::spawnEnemyFighter(const ca::Vec2& pos) {
    auto entityId = m_entities.createEntity();
    ju::Entity* entity = m_entities.getEntity(entityId);

    // PositionComponent
    entity->addComponent<PositionComponent>(pos);

    // MovementComponent
    auto movement = entity->addComponent<MovementComponent>();
    movement->maxSpeed = 2.5f;

    // CollisionComponent
    auto collision = entity->addComponent<CollisionComponent>();
    collision->collisionRadius = 32.f;

    // SpriteComponent
    auto sprite = entity->addComponent<SpriteComponent>();
    sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_enemyFighterTexture.get());
    // sprite->movementTarget = nu::MakeScopedPtr<ca::Sprite>(m_movementTargetTexture.get());

    // CombatComponent
    auto combat = entity->addComponent<CombatComponent>();
    combat->faction = FactionFoe;
    combat->diesOnCollision = false;
    combat->health = 25.f;
    combat->fireRate = 1.f;
    combat->fireRange = 500.f;

    return true;
  }

  bool World::spawnAsteroid(const ca::Vec2& pos, F32 direction) {
    auto entityId = m_entities.createEntity();
    ju::Entity* entity = m_entities.getEntity(entityId);

    // PositionComponent
    auto position = entity->addComponent<PositionComponent>(pos);
    position->direction = direction;

    // AsteroidComponent
    auto asteroid = entity->addComponent<AsteroidComponent>();

    // SpriteComponent
    auto sprite = entity->addComponent<SpriteComponent>();
    sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_asteroidTexture.get());

    return true;
  }

  bool World::spawnBarge(const ca::Vec2& pos) {
    auto entityId = m_entities.createEntity();
    ju::Entity* entity = m_entities.getEntity(entityId);

    // PositionComponent
    auto position = entity->addComponent<PositionComponent>(pos);

    // MovementComponent
    auto movement = entity->addComponent<MovementComponent>();
    movement->maxSpeed = 2.5f;

    // TargetComponent
    auto target = entity->addComponent<TargetComponent>();

    // MinerComponent
    auto miner = entity->addComponent<MinerComponent>();

    // SpriteComponent
    auto sprite = entity->addComponent<SpriteComponent>();
    sprite->icon = nu::makeScopedPtr<ca::Sprite>(m_bargeTexture.get());

    return true;
  }

  void World::posessEntity(ju::EntityId id) {
    m_posessedEntity.id = id;
  }
