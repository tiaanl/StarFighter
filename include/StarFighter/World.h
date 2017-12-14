
#ifndef STAR_FIGHTER_WORLD_H_
#define STAR_FIGHTER_WORLD_H_

#include "StarFighter/Camera.h"
#include "canvas/Math/Mat4.h"
#include "canvas/Rendering/Canvas.h"
#include "canvas/Rendering/Geometry.h"
#include "canvas/Rendering/Texture.h"
#include "canvas/Windows/KeyboardEventHandlerInterface.h"
#include "canvas/Windows/MouseEventHandlerInterface.h"
#include "canvas/Windows/Event.h"
#include "junctions/EntityManager.h"
#include "junctions/SystemManager.h"
#include "nucleus/Files/FilePath.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Memory/ScopedRefPtr.h"

class World : public ca::MouseEventHandlerInterface, public ca::KeyboardEventHandlerInterface {
public:
  World();
  ~World();

  bool create();

  ju::Entity* getPlayerEntity() {
    return m_entities.getEntity(m_starFighterEntityId);
  }

  void setViewportSize(const ca::Size<U32>& viewportSize);
  void update(ca::Canvas* canvas, const ca::Size<U32>& viewportSize, F32 adjustment);

  void onMouseMoved(const ca::MouseEvent& evt) override;
  bool onMousePressed(const ca::MouseEvent& evt) override;
  void onMouseReleased(const ca::MouseEvent& evt) override;
  void onMouseWheel(const ca::MouseWheelEvent& evt) override;

  void onKeyPressed(const ca::KeyEvent& evt) override;
  void onKeyReleased(const ca::KeyEvent& evt) override;

private:
  nu::ScopedPtr<ca::Texture> createTexture(const nu::FilePath& filename);

  bool spawnStarFighter();

  bool spawnMouseCursor();
  bool spawnEnemyFighter(const ca::Vec2& pos);
  bool spawnAsteroid(const ca::Vec2& pos, F32 direction);
  bool spawnBarge(const ca::Vec2& pos);

  void posessEntity(ju::EntityId id);

  ca::Size<U32> m_viewportSize;

  Camera m_camera;

  ca::Pos<I32> m_mousePosition;

  ju::EntityManager m_entities;
  ju::SystemManager m_systems;

  // Some textures.
  nu::ScopedPtr<ca::Texture> m_mousePointerTexture;
  nu::ScopedPtr<ca::Texture> m_starFighterTexture;
  nu::ScopedPtr<ca::Texture> m_enemyFighterTexture;
  nu::ScopedPtr<ca::Texture> m_movementTargetTexture;
  nu::ScopedPtr<ca::Texture> m_asteroidTexture;
  nu::ScopedPtr<ca::Texture> m_bargeTexture;

  ju::EntityId m_starFighterEntityId{ju::kInvalidEntityId};
  ju::EntityId m_mousePointerEntityId{ju::kInvalidEntityId};

  bool m_settingCameraPos{false};
  ca::Pos<I32> m_cameraPosStartPos;

  ca::Vec2 m_mouseWorldPos;

  struct PosessedEntity {
    bool isSettingMovementTarget{false};
    ju::EntityId id{ju::kInvalidEntityId};
  } m_posessedEntity;

  DISALLOW_COPY_AND_ASSIGN(World);
};

#endif  // STAR_FIGHTER_WORLD_H_
