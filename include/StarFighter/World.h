
#ifndef STAR_FIGHTER_WORLD_H_
#define STAR_FIGHTER_WORLD_H_

#include <memory>

#include "StarFighter/Camera.h"
#include "canvas/Math/Mat4.h"
#include "canvas/Rendering/Canvas.h"
#include "canvas/Rendering/Geometry.h"
#include "canvas/Rendering/Texture.h"
#include "canvas/Resources/ResourceCache.h"
#include "canvas/Windows/Event.h"
#include "canvas/Windows/KeyboardEventHandlerInterface.h"
#include "canvas/Windows/MouseEventHandlerInterface.h"
#include "nucleus/FilePath.h"

class World : public ca::MouseEventHandlerInterface, public ca::KeyboardEventHandlerInterface {
public:
  explicit World() = default;
  ~World() = default;

  bool create();

  void setViewportSize(const ca::Size<U32>& viewportSize);
  void update(ca::Canvas* canvas, const ca::Size<U32>& viewportSize, F32 adjustment);

  void onMouseMoved(const ca::MouseEvent& evt) override;
  bool onMousePressed(const ca::MouseEvent& evt) override;
  void onMouseReleased(const ca::MouseEvent& evt) override;
  void onMouseWheel(const ca::MouseWheelEvent& evt) override;

  void onKeyPressed(const ca::KeyEvent& evt) override;
  void onKeyReleased(const ca::KeyEvent& evt) override;

private:
  std::unique_ptr<ca::Texture> createTexture(const nu::FilePath& filename);

  ca::Size<U32> m_viewportSize;

  Camera m_camera;

  ca::Pos<I32> m_mousePosition;

  // Some textures.
  ca::Texture* m_mousePointerTexture;
  ca::Texture* m_starFighterTexture;
  ca::Texture* m_enemyFighterTexture;
  ca::Texture* m_movementTargetTexture;
  ca::Texture* m_asteroidTexture;
  ca::Texture* m_bargeTexture;
  ca::Texture* m_hoverTexture;
  ca::Texture* m_selectedTexture;

  ca::Vec2 m_mouseWorldPos;

  struct CameraMovement {
    bool isSettingPos{false};
    ca::Pos<I32> startPos;
  } m_cameraMovement;
};

#endif  // STAR_FIGHTER_WORLD_H_
