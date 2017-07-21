
#ifndef STAR_FIGHTER_SYSTEMS_RENDER_SYSTEM_H_
#define STAR_FIGHTER_SYSTEMS_RENDER_SYSTEM_H_

#include "canvas/math/mat4.h"
#include "canvas/rendering/canvas.h"
#include "junctions/entity_manager.h"

class RenderSystem {
public:
    void configure(ju::EntityManager& events);
    void update(ju::EntityManager& entities, ca::Canvas* canvas, const ca::Mat4& mat);

private:
    DISALLOW_COPY_AND_ASSIGN(RenderSystem);
};

#endif  // STAR_FIGHTER_SYSTEMS_RENDER_SYSTEM_H_
