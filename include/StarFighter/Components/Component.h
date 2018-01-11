
#ifndef STAR_FIGHTER_COMPONENTS_COMPONENT_H_
#define STAR_FIGHTER_COMPONENTS_COMPONENT_H_

#include "canvas/Rendering/Canvas.h"
#include "nucleus/Types.h"

class Component {
public:
  Component() = default;
  virtual ~Component() = default;

  virtual void onUpdate(F32 delta) = 0;
  virtual void onRender(ca::Canvas* canvas, const ca::Mat4& mat) = 0;
};

#endif  // STAR_FIGHTER_COMPONENTS_COMPONENT_H_
