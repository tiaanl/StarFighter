
#include "StarFighter/World.h"

#include <filesystem>
#include <random>

#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "StarFighter/Systems/MovementSystem.h"
#include "StarFighter/Systems/RenderSystem.h"
#include "canvas/Math/Transform.h"
#include "nucleus/Files.h"
#include "nucleus/Streams/FileInputStream.h"

#include "nucleus/MemoryDebug.h"

bool World::create() {
  // Load all our textures.
  m_starFighterTexture = createTexture(nu::FilePath("StarFighter.png"));

#if 0
  m_mousePointerTexture = createTexture(nu::FilePath{"MouseCursor.png"});
  m_starFighterTexture = createTexture(nu::FilePath{"StarFighter.png"});
  m_enemyFighterTexture = createTexture(nu::FilePath{"Enemy1.png"});
  m_movementTargetTexture = createTexture(nu::FilePath{"MovementTarget.png"});
  m_asteroidTexture = createTexture(nu::FilePath{"asteroid-1.png"});
  m_bargeTexture = createTexture(nu::FilePath{"mining-barge.png"});
  m_hoverTexture = createTexture(nu::FilePath{"hover.png"});
  m_selectedTexture = createTexture(nu::FilePath{"selected.png"});
#endif  // 0

  m_entities.emplace_back();
  Entity& last = m_entities.back();
  last.transform().rot = ca::Vec3{0.f, 0.f, 30.f};

  auto sprite1 = last.emplaceComponent<SpriteComponent>();
  sprite1->setTexture(m_starFighterTexture.get());

  auto sprite2 = last.emplaceComponent<SpriteComponent>();
  sprite2->setTexture(m_starFighterTexture.get());
  sprite2->transform().pos = ca::Vec3{250.f, 250.f, 0.f};

  return true;
}

void World::setViewportSize(const ca::Size<U32>& viewportSize) {
  m_viewportSize = viewportSize;
}

void World::update(F32 adjustment) {
  // Update the camera animations.
  m_camera.update(adjustment);

  // Get the world position of where the mouse is currently.
  ca::Pos<F32> mousePos(static_cast<F32>(m_mousePosition.x), static_cast<F32>(m_mousePosition.y));
  m_mouseWorldPos = m_camera.unproject(m_viewportSize, mousePos);

  for (auto& entity : m_entities) {
    entity.onUpdate(adjustment);
  }
}

void World::render(ca::Canvas* canvas) {
  // Get the final version of the camera's model/view matrix.
  ca::Mat4 mat = m_camera.calculateMatrix(m_viewportSize);

  for (auto& entity : m_entities) {
    entity.onRender(canvas, mat);
  }
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
  nu::FilePath root = nu::getCurrentWorkingDirectory().append(nu::FilePath{"assets"});

  nu::FileInputStream stream(root.append(filename));

  ca::Image image;
  if (!image.loadFromStream(&stream)) {
    LOG(Warning) << "Could not load image.";
    return nullptr;
  }

  auto texture = std::make_unique<ca::Texture>();
  texture->createFromImage(image);

  return texture;
}
