
#ifndef STAR_FIGHTER_EVENTS_INPUT_EVENT_H_
#define STAR_FIGHTER_EVENTS_INPUT_EVENT_H_

#include "junctions/EntityManager.h"

struct InputEvent {
  ju::EntityId id;

  enum Action {
    ActionStartFiring,
    ActionStopFiring,
  } action;

  InputEvent(ju::EntityId id, Action action) : id(id), action(action) {}
};

#endif  // STAR_FIGHTER_EVENTS_INPUT_EVENT_H_
