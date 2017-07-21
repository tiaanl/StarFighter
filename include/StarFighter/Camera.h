
#ifndef STARFIGHTER_CAMERA_H_
#define STARFIGHTER_CAMERA_H_

#include "canvas/math/mat4.h"
#include "canvas/utils/pos.h"
#include "canvas/utils/size.h"
#include "nucleus/macros.h"

class Camera {
public:
    Camera();
    ~Camera();

    const ca::Pos<F32>& getPosition() const { return m_pos; }
    void setPosition(const ca::Pos<F32>& pos);

    F32 getZoom() const { return m_zoom; }
    void setZoom(F32 m_zoom);

    ca::Mat4 calculateMatrix(const ca::Size<U32>& viewportSize) const;
    ca::Vec2 unproject(const ca::Size<U32>& viewportSize, const ca::Pos<F32>& pos) const;

    void zoomTo(F32 zoom);
    void update(F32 adjustment);

private:
    // The current position of the camera in world space.
    ca::Pos<F32> m_pos{0.f, 0.f};

    // The position in the world where the camera is moving to.
    ca::Pos<F32> m_targetPos{0.f, 0.f};

    // The current zoom level of the camera.
    F32 m_zoom{1.f};

    // The zoom level that we are zooming to.
    F32 m_targetZoom{1.f};

    DISALLOW_COPY_AND_ASSIGN(Camera);
};

#endif  // STARFIGHTER_CAMERA_H_
