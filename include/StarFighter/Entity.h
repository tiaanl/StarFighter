
#ifndef STAR_FIGHTER_ENTITY_H_
#define STAR_FIGHTER_ENTITY_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "StarFighter/Components/Component.h"
#include "StarFighter/Transform.h"

class Entity {
public:
  Entity() = default;
  ~Entity() = default;

  Entity(Entity&& other) : m_components(std::move(other.m_components)) {}

  Entity& operator=(Entity&& other) {
    m_components = std::move(other.m_components);

    return *this;
  }

  Transform& transform() {
    return m_transform;
  }

  template <typename ComponentType, typename... Args>
  ComponentType* emplaceComponent(Args... args) {
    auto newComponent = std::make_unique<ComponentType>(std::forward<Args>(args)...);
    m_components.push_back(std::move(newComponent));
    return static_cast<ComponentType*>(m_components.back().get());
  }

  void onUpdate(F32 delta) {
    for (auto& component : m_components) {
      component->onUpdate(delta);
    }
  }

  void onRender(ca::Canvas* canvas, const ca::Mat4& mat) {
    for (auto& component : m_components) {
      component->onRender(canvas, m_transform.applyTo(mat));
    }
  }

private:
  COPY_DELETE(Entity);

  Transform m_transform;

  std::vector<std::unique_ptr<Component>> m_components;
};

#endif  // STAR_FIGHTER_ENTITY_H_
