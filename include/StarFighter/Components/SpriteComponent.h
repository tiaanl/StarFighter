
#ifndef START_FIGHTER_COMPONENTS_SPRITE_COMPONENT_H_
#define START_FIGHTER_COMPONENTS_SPRITE_COMPONENT_H_

#include "StarFighter/Components/Component.h"
#include "canvas/Primitives/Sprite.h"

class SpriteComponent : public Component {
public:
  SpriteComponent() = default;
  virtual ~SpriteComponent() = default;

  Transform& transform() {
    return m_transform;
  }

  void setTexture(ca::Texture* texture) {
    m_sprite.setTexture(texture);
  }

  void onUpdate(F32 delta) override {}

  void onRender(ca::Canvas* canvas, const ca::Mat4& mat) override {
    m_sprite.render(canvas, m_transform.applyTo(mat));
  }

private:
  Transform m_transform;
  ca::Sprite m_sprite;
};

#endif  // START_FIGHTER_COMPONENTS_SPRITE_COMPONENT_H_
