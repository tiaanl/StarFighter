
#include "StarFighter/World.h"

#include <random>

#include "StarFighter/Components/CollisionComponent.h"
#include "StarFighter/Components/CombatComponent.h"
#include "StarFighter/Components/MovementComponent.h"
#include "StarFighter/Components/PositionComponent.h"
#include "StarFighter/Components/SpriteComponent.h"
#include "StarFighter/Systems/CollisionSystem.h"
#include "StarFighter/Systems/CombatSystem.h"
#include "StarFighter/Systems/MovementSystem.h"
#include "StarFighter/Systems/RenderSystem.h"
#include "canvas/math/transform.h"
#include "nucleus/streams/file_input_stream.h"

World::World() : m_entities{}, m_systems{&m_entities} {}

World::~World() {}

bool World::create() {
    // Add the systems.
    m_systems.addSystem<CombatSystem>(
        createTexture(nu::FilePath{FILE_PATH_LITERAL("C:\\Code\\StarFighter\\assets\\Bullet.png")}));
    m_systems.addSystem<MovementSystem>();
    m_systems.addSystem<CollisionSystem>();
    m_systems.addSystem<RenderSystem>();

    // Load all our textures.
    m_mousePointerTexture =
        createTexture(nu::FilePath{FILE_PATH_LITERAL("C:\\Code\\StarFighter\\assets\\MouseCursor.png")});
    m_starFighterTexture =
        createTexture(nu::FilePath{FILE_PATH_LITERAL("C:\\Code\\StarFighter\\assets\\StarFighter.png")});
    m_enemyFighterTexture = createTexture(nu::FilePath{FILE_PATH_LITERAL("C:\\Code\\StarFighter\\assets\\Enemy1.png")});
    m_movementTargetTexture =
        createTexture(nu::FilePath{FILE_PATH_LITERAL("C:\\Code\\StarFighter\\assets\\MovementTarget.png")});

    if (!spawnStarFighter()) {
        return false;
    }

    if (!spawnMouseCursor()) {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000, 1000);

    for (auto i = 0; i < 20; ++i) {
        if (!spawnEnemyFighter(ca::Vec2{static_cast<F32>(dis(gen)), static_cast<F32>(dis(gen))})) {
            return false;
        }
    }

    return true;
}

void World::setViewportSize(const ca::Size<U32>& viewportSize) {
    m_viewportSize = viewportSize;
}

void World::update(ca::Canvas* canvas, const ca::Size<U32>& viewportSize, F32 adjustment) {
    // Update the camera animations.
    m_camera.update(adjustment);

    // Get the final version of the camera's model/view matrix.
    ca::Mat4 mat = m_camera.calculateMatrix(viewportSize);

    // Update the mouse position in the world.
    ca::Pos<F32> mousePos(static_cast<F32>(m_mousePosition.x), static_cast<F32>(m_mousePosition.y));
    ca::Vec2 positionInWorld = m_camera.unproject(m_viewportSize, mousePos);

    auto position = m_entities.getComponent<PositionComponent>(m_mousePointerEntityId);
    position->pos = ca::Vec2{positionInWorld.x, positionInWorld.y};

    if (m_settingStarFighterPos) {
        auto movement = m_entities.getComponent<MovementComponent>(m_starFighterEntityId);
        movement->moveToPos(positionInWorld, movement->maxSpeed);
    }

    // Decide what each entity wants to do.
    m_systems.update<CombatSystem>(adjustment);

    // Move each entity into next step position according to what we decided it wants to do.
    m_systems.update<MovementSystem>(adjustment);

    // Check collision between entities that just moved.
    m_systems.update<CollisionSystem>();

    // Render all the entities.
    m_systems.update<RenderSystem>(canvas, mat);

    // Perform any updates on the entity manager.
    m_entities.update();

    // After all the entities have been moved, move the camera to the same position as the StarFighter.
    {
        auto starFighterPos = m_entities.getComponent<PositionComponent>(m_starFighterEntityId);
        m_camera.setPosition(ca::Pos<F32>{-starFighterPos->pos.x, -starFighterPos->pos.y});
    }
}

void World::onMouseMoved(const ca::MouseEvent& evt) {
    m_mousePosition = evt.pos;
}

bool World::onMousePressed(const ca::MouseEvent& evt) {
    if (evt.button == ca::MouseEvent::Button::Left) {
        // This will be set to true as long as the mouse button is pressed down.
        m_settingStarFighterPos = true;

        // We want to receive an onMouseRelease event.
        return true;
    }

    return false;
}

void World::onMouseReleased(const ca::MouseEvent& evt) {
    if (m_settingStarFighterPos && evt.button == ca::MouseEvent::Left) {
        m_settingStarFighterPos = false;
    }
}

void World::onMouseWheel(const ca::MouseWheelEvent& evt) {
    m_camera.zoomTo(m_camera.getZoom() + static_cast<F32>(evt.wheelOffset.y) * 0.1f);
}

void World::onKeyPressed(const ca::KeyEvent& evt) {
    if (evt.key == ca::Space) {
        m_entities.emit<InputEvent>(m_starFighterEntityId, InputEvent::ActionStartFiring);
    }
}

void World::onKeyReleased(const ca::KeyEvent& evt) {
    if (evt.key == ca::Space) {
        m_entities.emit<InputEvent>(m_starFighterEntityId, InputEvent::ActionStopFiring);
    }
}

nu::ScopedPtr<ca::Texture> World::createTexture(const nu::FilePath& filename) {
    nu::FileInputStream stream(filename);

    ca::Image image;
    if (!image.loadFromStream(&stream)) {
        LOG(Warning) << "Could not load ship image.";
        return nullptr;
    }

    auto texture = nu::MakeScopedPtr<ca::Texture>();
    texture->createFromImage(image);

    return texture;
}

bool World::spawnStarFighter() {
    m_starFighterEntityId = m_entities.createEntity();
    ju::Entity* entity = m_entities.getEntity(m_starFighterEntityId);

    // PositionComponent
    entity->addComponent<PositionComponent>();

    // MovementComponent
    auto movement = entity->addComponent<MovementComponent>();
    movement->currentSpeed = 0.f;
    movement->maxSpeed = 5.f;

    // CollisionComponent
    auto collision = entity->addComponent<CollisionComponent>();
    collision->collisionRadius = 32.f;

    // SpriteComponent
    auto sprite = entity->addComponent<SpriteComponent>();
    sprite->icon = nu::MakeScopedPtr<ca::Sprite>(m_starFighterTexture.get());

    // CombatComponent
    auto combat = entity->addComponent<CombatComponent>();
    combat->combatMode = CombatComponent::ModePlayer;
    combat->faction = FactionFriend;
    combat->fireRate = 0.1f;
    combat->fireRange = 1000.f;
    combat->diesOnCollision = false;
    combat->health = 100000.f;

    return true;
}

bool World::spawnMouseCursor() {
    m_mousePointerEntityId = m_entities.createEntity();
    auto mousePointerEntity = m_entities.getEntity(m_mousePointerEntityId);
    mousePointerEntity->addComponent<PositionComponent>();

    auto sprite = mousePointerEntity->addComponent<SpriteComponent>();
    sprite->icon = nu::MakeScopedPtr<ca::Sprite>(m_mousePointerTexture.get());

    return true;
}

bool World::spawnEnemyFighter(const ca::Vec2& pos) {
    auto entityId = m_entities.createEntity();
    ju::Entity* entity = m_entities.getEntity(entityId);

    // PositionComponent
    entity->addComponent<PositionComponent>(pos);

    // MovementComponent
    auto movement = entity->addComponent<MovementComponent>();
    movement->maxSpeed = 2.5f;

    // CollisionComponent
    auto collision = entity->addComponent<CollisionComponent>();
    collision->collisionRadius = 32.f;

    // SpriteComponent
    auto sprite = entity->addComponent<SpriteComponent>();
    sprite->icon = nu::MakeScopedPtr<ca::Sprite>(m_enemyFighterTexture.get());
    // sprite->movementTarget = nu::MakeScopedPtr<ca::Sprite>(m_movementTargetTexture.get());

    // CombatComponent
    auto combat = entity->addComponent<CombatComponent>();
    combat->faction = FactionFoe;
    combat->diesOnCollision = false;
    combat->health = 25.f;
    combat->fireRate = 1.f;
    combat->fireRange = 500.f;

    return true;
}
