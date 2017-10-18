
#ifndef STAR_FIGHTER_SYSTEMS_COMBAT_SYSTEM_H_
#define STAR_FIGHTER_SYSTEMS_COMBAT_SYSTEM_H_

#include "StarFighter/Events/InputEvent.h"
#include "canvas/rendering/texture.h"
#include "junctions/entity_manager.h"
#include "nucleus/Memory/ScopedPtr.h"

class CombatSystem {
public:
  explicit CombatSystem(nu::ScopedPtr<ca::Texture> bulletTexture);

  void configure(ju::EntityManager& entities);
  void update(ju::EntityManager& entities, F32 adjustment);
  void receive(ju::EntityManager& entities, const InputEvent& evt);

private:
  void handleModeIdle(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) const;
  void handleModeChaseTarget(ju::EntityManager& entities, ju::Entity& entity, F32 adjustment) const;

  void spawnBullet(ju::EntityManager& entities, ju::Entity& source) const;

  nu::ScopedPtr<ca::Texture> m_bulletTexture;
};

#endif  // STAR_FIGHTER_SYSTEMS_COMBAT_SYSTEM_H_
