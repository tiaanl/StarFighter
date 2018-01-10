
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

bool World::create() {
#if 0
  // Load all our textures.
  m_mousePointerTexture = m_resourceCache->getTexture("MouseCursor.png");
  m_starFighterTexture = m_resourceCache->getTexture("StarFighter.png");
  m_enemyFighterTexture = m_resourceCache->getTexture("Enemy1.png");
  m_movementTargetTexture = m_resourceCache->getTexture("MovementTarget.png");
  m_asteroidTexture = m_resourceCache->getTexture("asteroid-1.png");
  m_bargeTexture = m_resourceCache->getTexture("mining-barge.png");
  m_hoverTexture = m_resourceCache->getTexture("hover.png");
  m_selectedTexture = m_resourceCache->getTexture("selected.png");
#endif  // 0

  // if (!spawnStarFighter()) {
  //  return false;
  //}

  // m_hoverEntityId = spawnHoverEntity();
  // m_selectedEntityId = spawnSelectedEntity();

  // if (!spawnMouseCursor()) {
  //   return false;
  // }

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

#if 0
  for (auto i = 0; i < 20; ++i) {
    if (!spawnAsteroid(ca::Vec2{static_cast<F32>(intDist(gen)), static_cast<F32>(intDist(gen))},
                       static_cast<F32>(realDist(gen)))) {
      return false;
    }
  }
#endif  // 0

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

  // Get the world position of where the mouse is currently.
  ca::Pos<F32> mousePos(static_cast<F32>(m_mousePosition.x), static_cast<F32>(m_mousePosition.y));
  m_mouseWorldPos = m_camera.unproject(m_viewportSize, mousePos);

  // Update the mouse entity's position in the world.
  // auto position = m_entities.getComponent<PositionComponent>(m_mousePointerEntityId);
  // position->pos = ca::Vec2{m_mouseWorldPos.x, m_mouseWorldPos.y};
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
  return false;
}

void World::onMouseReleased(const ca::MouseEvent& evt) {
  if (m_cameraMovement.isSettingPos && evt.button == ca::MouseEvent::Right) {
    m_cameraMovement.isSettingPos = false;
  }
}

void World::onMouseWheel(const ca::MouseWheelEvent& evt) {
  m_camera.zoomTo(m_camera.getZoom() + static_cast<F32>(evt.wheelOffset.y) * 0.1f);
}

void World::onKeyPressed(const ca::KeyEvent& evt) {}

void World::onKeyReleased(const ca::KeyEvent& evt) {}

std::unique_ptr<ca::Texture> World::createTexture(const nu::FilePath& filename) {
  nu::FileInputStream stream(filename);

  ca::Image image;
  if (!image.loadFromStream(&stream)) {
    LOG(Warning) << "Could not load ship image.";
    return nullptr;
  }

  auto texture = std::make_unique<ca::Texture>();
  texture->createFromImage(image);

  return texture;
}
