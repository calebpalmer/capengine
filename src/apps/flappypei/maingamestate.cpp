#include "maingamestate.h"

#include <SDL_events.h>
#include <capengine/CapEngineException.h>
#include <capengine/collision.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>

#include <memory>

#include "constants.h"
#include "playergraphicscomponent.h"
#include "playerphysicscomponent.h"

namespace FlappyPei {

namespace {

std::unique_ptr<CapEngine::GameObject> createPlayerObject(uint32_t in_windowId)
{
    auto playerObject = std::make_unique<CapEngine::GameObject>();
    playerObject->addComponent(std::make_shared<FlappyPei::PlayerGraphicsComponent>());
    playerObject->addComponent(std::make_shared<FlappyPei::PlayerPhysicsComponent>());

    CapEngine::Vector initialPosition((kLogicalWindowWidth / 2.0) + (kSpriteWidth / 2.0),
                                      (kLogicalWindowHeight / 2.0) + (kSpriteHeight / 2.0));
    playerObject->setPosition(initialPosition);  // Set initial position
    playerObject->setObjectState(CapEngine::GameObject::ObjectState::Starting);

    return playerObject;
}
}  // namespace

/**
 * \brief Constructor
 * \param in_windowId The window id.
 */
MainGameState::MainGameState(uint32_t in_windowId)
    : CapEngine::GameState(), m_windowId(in_windowId), m_camera({kLogicalWindowHeight, kLogicalWindowWidth})
{
    // register for keyboard events
    CapEngine::Locator::getEventSubscriber().m_keyboardEventSignal.connect(
        [this](SDL_KeyboardEvent in_event) { this->handleKeyboardEvent(in_event); });
}

/**
 * \brief Destructor
 */
bool MainGameState::onLoad()
{
    m_playerObject = createPlayerObject(m_windowId);
    return true;
}

/**
 * \brief Renders the game state.
 */
void MainGameState::render()
{
    if (m_playerObject) {
        m_playerObject->render(m_camera, m_windowId);
    }

    // FPS
    // CapEngine::Locator::getVideoManager().displayFPS(true, "tahoma.ttf", 255, 255, 255);
}

/**
 * \brief Updates the game state.
 * \param timestepMs The timestep in milliseconds.
 */
void MainGameState::update(double timestepMs)
{
    m_telemetry.elapsedTimeMs += timestepMs;
    if (m_telemetry.elapsedTimeMs >= kGameWaitTime) {
        // set the game status
        m_gameState.status = GameStatus::Active;

        // start the player
        if (m_playerObject->getObjectState() == CapEngine::GameObject::ObjectState::Starting) {
            m_playerObject->setObjectState(CapEngine::GameObject::ObjectState::Active);
        }
    }

    CAP_THROW_NULL(m_playerObject, "Player object is null");
    std::unique_ptr<CapEngine::GameObject> newPlayerObject = m_playerObject->update(timestepMs);
    CAP_THROW_NULL(newPlayerObject, "New player object is null");

    // check collision with window and player object
    const CapEngine::Rectangle windowRect{0, 0, kLogicalWindowWidth, kLogicalWindowHeight};
    const CapEngine::CollisionType collisionType =
        CapEngine::detectMBRCollisionInterior(newPlayerObject->boundingPolygon(), windowRect);

    if (collisionType == CapEngine::CollisionType::COLLISION_NONE) {
        m_playerObject = m_playerObject->update(timestepMs);
    }
}

/**
 * \brief Handles keyboard events.
 * \param event The keyboard event.
 */
void MainGameState::handleKeyboardEvent(const SDL_KeyboardEvent& event)
{
}

}  // namespace FlappyPei
