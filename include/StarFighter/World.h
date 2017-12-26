
#ifndef STAR_FIGHTER_WORLD_H_
#define STAR_FIGHTER_WORLD_H_

#include "StarFighter/Camera.h"
#include "canvas/Math/Mat4.h"
#include "canvas/Rendering/Canvas.h"
#include "canvas/Rendering/Geometry.h"
#include "canvas/Rendering/Texture.h"
#include "canvas/Resources/ResourceCache.h"
#include "canvas/Windows/Event.h"
#include "canvas/Windows/KeyboardEventHandlerInterface.h"
#include "canvas/Windows/MouseEventHandlerInterface.h"
#include "junctions/EntityManager.h"
#include "junctions/SystemManager.h"
#include "nucleus/Files/FilePath.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Memory/ScopedRefPtr.h"

class World : public ca::MouseEventHandlerInterface, public ca::KeyboardEventHandlerInterface {
public:
  explicit World(ca::ResourceCache* resourceCache);
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

  ju::EntityId spawnHoverEntity();
  ju::EntityId spawnSelectedEntity();

  void posessEntity(ju::EntityId id);

  ju::EntityId getEntityUnderMouse(const ca::Vec2& mouseWorldPos);

  ca::ResourceCache* m_resourceCache;

  ca::Size<U32> m_viewportSize;

  Camera m_camera;

  ca::Pos<I32> m_mousePosition;

  ju::EntityManager m_entities;
  ju::SystemManager m_systems;

  // Some textures.
  ca::Texture* m_mousePointerTexture;
  ca::Texture* m_starFighterTexture;
  ca::Texture* m_enemyFighterTexture;
  ca::Texture* m_movementTargetTexture;
  ca::Texture* m_asteroidTexture;
  ca::Texture* m_bargeTexture;
  ca::Texture* m_hoverTexture;
  ca::Texture* m_selectedTexture;

  ju::EntityId m_starFighterEntityId{ju::kInvalidEntityId};
  ju::EntityId m_mousePointerEntityId{ju::kInvalidEntityId};
  ju::EntityId m_hoverEntityId{ju::kInvalidEntityId};
  ju::EntityId m_selectedEntityId{ju::kInvalidEntityId};

  ca::Vec2 m_mouseWorldPos;

  struct CameraMovement {
    bool isSettingPos{false};
    ca::Pos<I32> startPos;
  } m_cameraMovement;

  struct PosessedEntity {
    bool isSettingMovementTarget{false};
    ju::EntityId id{ju::kInvalidEntityId};
  } m_posessedEntity;

  struct Selection {
    ju::EntityId hoverEntityId;
    ju::EntityId selectedEntityId;
  } m_selection;

  DISALLOW_COPY_AND_ASSIGN(World);
};

#endif  // STAR_FIGHTER_WORLD_H_
