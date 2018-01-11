
#ifndef STAR_FIGHTER_TRANSFORM_H_
#define STAR_FIGHTER_TRANSFORM_H_

#include "canvas/Math/Transform.h"
#include "canvas/Math/Vec3.h"

struct Transform {
  ca::Vec3 pos{0.f, 0.f, 0.f};
  ca::Vec3 rot{0.f, 0.f, 0.f};
  ca::Vec3 scale{1.f, 1.f, 1.f};

  ca::Mat4 applyTo(const ca::Mat4& mat) {
    ca::Mat4 result = mat;

    result *= ca::translate(pos);
    result *= ca::rotate(ca::degreesToRadians(rot.x), ca::Vec3{1.f, 0.f, 0.0f});
    result *= ca::rotate(ca::degreesToRadians(rot.y), ca::Vec3{0.f, 1.f, 0.0f});
    result *= ca::rotate(ca::degreesToRadians(rot.z), ca::Vec3{0.f, 0.f, 1.0f});
    result *= ca::scale(scale);

    return result;
  }
};

#endif  // STAR_FIGHTER_TRANSFORM_H_
