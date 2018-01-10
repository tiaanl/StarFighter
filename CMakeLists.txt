cmake_minimum_required(VERSION 3.7.2)

project(StarFighter)

# Options

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 14)

# Dependencies

include(cmake/elastic.cmake)

set("StarFighter_INCLUDE_FILES"
        "include/StarFighter/Camera.h"
        "include/StarFighter/Data.h"
        "include/StarFighter/UIContext.h"
        "include/StarFighter/World.h"
        "include/StarFighter/Components/PositionComponent.h"
        "include/StarFighter/Components/SpriteComponent.h"
        "include/StarFighter/Components/MovementComponent.h"
        "include/StarFighter/Events/InputEvent.h"
        "include/StarFighter/Systems/RenderSystem.h"
        "include/StarFighter/Systems/MovementSystem.h"
        )

set("StarFighter_SOURCE_FILES"
        "src/App.cpp"
        "src/Camera.cpp"
        "src/UIContext.cpp"
        "src/World.cpp"
        "src/Systems/RenderSystem.cpp"
        "src/Systems/MovementSystem.cpp"
        )

add_executable(StarFighter WIN32 ${StarFighter_INCLUDE_FILES} ${StarFighter_SOURCE_FILES})
target_include_directories(StarFighter PRIVATE "include")
target_link_libraries(StarFighter canvas elastic)
target_include_directories(StarFighter PRIVATE "${PROJECT_SOURCE_DIR}/include")
