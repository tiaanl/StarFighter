
#include "StarFighter/Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "canvas/Math/Transform.h"

#include "nucleus/MemoryDebug.h"

void Camera::setPosition(const ca::Pos<F32>& pos) {
  m_pos = pos;
}

void Camera::setZoom(F32 zoom) {
  m_zoom = std::max(std::min(zoom, 2.f), 0.25f);
}

ca::Mat4 Camera::calculateMatrix(const ca::Size<U32>& viewportSize) const {
  F32 halfWidth = static_cast<F32>(viewportSize.width) / 2.f;
  F32 halfHeight = static_cast<F32>(viewportSize.height) / 2.f;

  ca::Mat4 projection = ca::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);

  ca::Mat4 view;
  view *= ca::scale(m_zoom, m_zoom, 1.f);
  view *= ca::rotate(static_cast<F32>(M_PI), ca::Vec3(0.f, 0.f, 1.f));
  view *= ca::translate(m_pos.x, m_pos.y, 0.f);

  ca::Mat4 finalMatrix = projection * view;

  return finalMatrix;
}

ca::Vec2 Camera::unproject(const ca::Size<U32>& viewportSize, const ca::Pos<F32>& pos) const {
  ca::Mat4 mat = calculateMatrix(viewportSize);

  ca::Vec4 result = ca::inverse(mat) * ca::Vec4(pos.x / static_cast<F32>(viewportSize.width) * 2.f - 1.f,
                                                -pos.y / static_cast<F32>(viewportSize.height) * 2.f + 1.f, 0.f, 1.f);

  return ca::Vec2{result.x, result.y};
}

void Camera::zoomTo(F32 zoom) {
  m_targetZoom = zoom;
}

void Camera::update(F32 adjustment) {
  setZoom(m_zoom + (m_targetZoom - m_zoom) * 0.1f * adjustment);
}
