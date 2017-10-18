cmake_minimum_required(VERSION 3.7.2)

project(StarFighter)

include("${NUCLEUS_ROOT_DIR}/cmake/gcc.cmake")
include("${NUCLEUS_ROOT_DIR}/cmake/msvc.cmake")
include("${NUCLEUS_ROOT_DIR}/cmake/win32.cmake")

include(cmake/elastic.cmake)
include(cmake/junctions.cmake)

include_directories("include")

set("StarFighter_INCLUDE_FILES"
        "include/StarFighter/Camera.h"
        "include/StarFighter/Data.h"
        "include/StarFighter/UIContext.h"
        "include/StarFighter/World.h"

        "include/StarFighter/Components/AmmoComponent.h"
        "include/StarFighter/Components/AsteroidComponent.h"
        "include/StarFighter/Components/CollisionComponent.h"
        "include/StarFighter/Components/CombatComponent.h"
        "include/StarFighter/Components/PositionComponent.h"
        "include/StarFighter/Components/SpriteComponent.h"
        "include/StarFighter/Components/MovementComponent.h"
        "include/StarFighter/Components/MinerComponent.h"
        "include/StarFighter/Components/TargetComponent.h"

        "include/StarFighter/Events/InputEvent.h"

        "include/StarFighter/Systems/CombatSystem.h"
        "include/StarFighter/Systems/CollisionSystem.h"
        "include/StarFighter/Systems/RenderSystem.h"
        "include/StarFighter/Systems/MiningSystem.h"
        "include/StarFighter/Systems/MovementSystem.h"
        )

set("StarFighter_SOURCE_FILES"
        "src/App.cpp"
        "src/Camera.cpp"
        "src/UIContext.cpp"
        "src/World.cpp"

        "src/Systems/CombatSystem.cpp"
        "src/Systems/CollisionSystem.cpp"
        "src/Systems/RenderSystem.cpp"
        "src/Systems/MiningSystem.cpp"
        "src/Systems/MovementSystem.cpp"
        )

add_executable("StarFighter" WIN32 ${StarFighter_INCLUDE_FILES} ${StarFighter_SOURCE_FILES})
target_link_libraries(StarFighter canvas elastic junctions)
