#include "maingamestate.h"

#include <SDL_events.h>
#include <apps/flappypei/catgraphicscomponent.h>
#include <apps/flappypei/gameevents.h>
#include <capengine/CapEngineException.h>
#include <capengine/collision.h>
#include <capengine/colour.h>
#include <capengine/gameobject.h>
#include <capengine/gamestate.h>
#include <capengine/locator.h>
#include <capengine/vector.h>

#include <algorithm>
#include <boost/throw_exception.hpp>
#include <cstdint>
#include <memory>
#include <random>
#include <ranges>
#include <string>

#include "ballgraphicscomponent.h"
#include "ballphysicscomponent.h"
#include "blockgraphicscomponent.h"
#include "constants.h"
#include "playergraphicscomponent.h"
#include "playerphysicscomponent.h"

namespace Breakout {

namespace {

//! Generates a random number in a given range.
/**
 \param in_min
   The minimum value of the range.
 \param in_max
   The maximum value of the range.
 \return
   A random number.
*/
int generateRandomNumber(int in_min, int in_max)
{
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());
    std::uniform_int_distribution<int> uniformDist(in_min, in_max);

    return uniformDist(randomEngine);
}

//! Creates the player game object.
/**
 \param in_windowId
   The window ID.
 \return
   The player game object.
*/
std::unique_ptr<CapEngine::GameObject> createPlayerObject(uint32_t in_windowId)
{
    auto playerObject = std::make_unique<CapEngine::GameObject>();
    const double paddleY = 160.0;
    CapEngine::Vector initialPosition{(kLogicalWindowWidth / 2.0) - (kPaddleWidth / 2.0), paddleY};
    playerObject->setPosition(initialPosition);  // Set initial position
    playerObject->setObjectState(CapEngine::GameObject::ObjectState::Starting);

    playerObject->addComponent(std::make_shared<PlayerGraphicsComponent>());
    playerObject->addComponent(std::make_shared<PlayerPhysicsComponent>());

    return playerObject;
}

//! Creates the ball game object.
/**
 \param in_windowId
   The window ID.
 \return
   The ball game object.
*/
std::unique_ptr<CapEngine::GameObject> createBallObject(uint32_t in_windowId)
{
    auto ballObject = std::make_unique<CapEngine::GameObject>();
    const double ballY = 90;
    CapEngine::Vector initialPosition{(kLogicalWindowWidth / 2.0) - (kPaddleWidth / 2.0), ballY};
    ballObject->setPosition(initialPosition);
    // ballObject->setObjectState(CapEngine::GameObject::ObjectState::Starting);

    ballObject->addComponent(std::make_shared<BallGraphicsComponent>(kBallDiameter, CapEngine::Colour{255, 255, 255}));
    ballObject->addComponent(std::make_shared<BallPhysicsComponent>(kBallDiameter));

    return ballObject;
}

//! Creates a block game object.
/**
 \param in_windowId
   The window ID.
 \param in_blockWidth
   The width of the block.
 \param in_position
   The position of the block.
 \return
   A block game object.
*/
std::unique_ptr<CapEngine::GameObject> createBlockObject(uint32_t in_windowId, const int in_blockWidth,
                                                         CapEngine::Vector in_position)
{
    auto blockObject = std::make_unique<CapEngine::GameObject>();
    blockObject->addComponent(
        std::make_shared<BlockGraphicsComponent>(in_blockWidth, kBlockHeight, CapEngine::Colour{0, 255, 0}));
    // blockObject->addComponent(std::make_shared<BlockPhysicsComponent>());

    // CapEngine::Vector initialPosition{(kLogicalWindowWidth / 2.0) - (kPaddleWidth / 2.0), 40.0};
    blockObject->setPosition(in_position);  // Set initial position
    blockObject->setObjectState(CapEngine::GameObject::ObjectState::Starting);

    return blockObject;
}

//! Creates all the block game objects.
/**
 \param in_windowId
   The window ID.
 \return
   A vector of block game objects.
*/
std::vector<std::unique_ptr<CapEngine::GameObject>> createBlockObjects(uint32_t in_windowId)
{
    std::vector<std::unique_ptr<CapEngine::GameObject>> blockObjects;

    // calculate the width of a block based on the width of the screen, the required gap, and the number of blocks
    const int totalHorizontalGap = (kGap * (kNumBlocks - 1)) + (2 * kGap);
    const int blockWidth = (kLogicalWindowWidth - totalHorizontalGap) / kNumBlocks;

    const int totalVerticalGap = kGap * (kNumRows + 1);
    if (totalVerticalGap + (kBlockHeight * kNumRows) >= (kLogicalWindowHeight / 2)) {
        BOOST_THROW_EXCEPTION(CapEngine::CapEngineException("Too many block rows."));
    }

    for (int row : std::views::iota(0, kNumRows)) {
        for (int col : std::views::iota(0, kNumBlocks)) {
            CapEngine::Vector position{(col * blockWidth) + (col * kGap) + kGap,
                                       (row * kBlockHeight) + (row * kGap) + kGap};
            blockObjects.emplace_back(createBlockObject(in_windowId, blockWidth, position));
        }
    }

    return blockObjects;
}

//! Renders a text banner in the middle of the screen.
/**
 \param in_windowId
   The window ID.
 \param in_text
   The text to render.
*/
void renderBanner(Uint32 in_windowId, std::string const& in_text)
{
    CapEngine::SurfacePtr surface = CapEngine::Locator::getFontManager().getTextSurface(
        kDefaultFont, in_text, kBannerFontSize, CapEngine::Colour{255, 255, 255});
    CAP_THROW_NULL(surface, "Text surface is null");

    bool freeSurface = false;
    CapEngine::VideoManager& videoManager = CapEngine::Locator::getVideoManager();
    CapEngine::TexturePtr texture = videoManager.createTextureFromSurfacePtr(surface.get(), freeSurface);

    auto [width, height] = videoManager.getTextureDims(texture.get());

    CapEngine::Rect dstRect{
        .x = (kLogicalWindowWidth / 2) - (width / 2),
        .y = (kLogicalWindowHeight / 2) - (height / 2),
        .w = width,
        .h = height,
    };
    videoManager.drawTexture(in_windowId, dstRect, texture.get());
}

void ballPaddleCollisionCheck(CapEngine::GameObject& in_paddle, CapEngine::GameObject const& in_ball)
{
}

}  // namespace

//! Constructor.
/**
 \param in_windowId
   The window id.
*/
MainGameState::MainGameState(uint32_t in_windowId)
    : CapEngine::GameState(),
      m_windowId(in_windowId),
      m_camera({kLogicalWindowHeight, kLogicalWindowWidth}),
      m_playerObject(createPlayerObject(m_windowId)),
      m_blockObjects(createBlockObjects(m_windowId)),
      m_ballObject(createBallObject(m_windowId))
{
    // register for keyboard events
    CapEngine::Locator::getEventSubscriber().m_keyboardEventSignal.connect(
        [this](SDL_KeyboardEvent in_event) { this->handleKeyboardEvent(in_event); });
}

//! Renders the game state.
void MainGameState::render()
{
    auto doAlways = [&]() {
        // render player
        CAP_THROW_NULL(m_playerObject);
        m_playerObject->render(m_camera, m_windowId);

        CAP_THROW_NULL(m_ballObject);
        m_ballObject->render(m_camera, m_windowId);

        // render blocks
        std::ranges::for_each(m_blockObjects, [&](auto&& block) {
            CAP_THROW_NULL(block);
            block->render(m_camera, m_windowId);
        });
    };

    auto handleStarting = [&]() {
        // display start timer
        std::string renderText =
            std::to_string(static_cast<int>(3 - static_cast<int>((m_timing.elapsedTimeMs / 1000.0))));

        renderBanner(m_windowId, renderText);
    };

    auto handleDead = [&]() { renderBanner(m_windowId, "Game Over!"); };

    auto handleWin = [&]() { renderBanner(m_windowId, "You Win!"); };

    doAlways();

    switch (m_gameState.status) {
        case GameStatus::Starting:
            handleStarting();
            break;
        case GameStatus::Dead:
            handleDead();
            break;
        case GameStatus::Win:
            handleWin();
        case GameStatus::Active:
            break;
    }
}

//! Updates the game state.
/**
 \param timestepMs
   The timestep in milliseconds.
*/
void MainGameState::update(double timestepMs)
{
    // update timeings
    m_timing.elapsedTimeMs += timestepMs;
    m_timing.currentLevelTimeMs += timestepMs;

    auto handleStartingState = [&]() {
        if (m_timing.elapsedTimeMs >= kGameWaitTimeMs) {
            // set the game status
            m_gameState.status = GameStatus::Active;

            // start the player
            CAP_THROW_NULL(m_playerObject);
            m_playerObject->setObjectState(CapEngine::GameObject::ObjectState::Active);
            CAP_THROW_NULL(m_ballObject);
            m_ballObject->setObjectState(CapEngine::GameObject::ObjectState::Active);
        }
    };

    auto handleActiveState = [&]() {
        // update player
        std::unique_ptr<CapEngine::GameObject> newPlayerObject = m_playerObject->update(timestepMs);
        CAP_THROW_NULL(newPlayerObject);

        // collision with window
        const CapEngine::Rectangle windowRect{0, 0, kLogicalWindowWidth, kLogicalWindowHeight};
        const CapEngine::CollisionType collisionType =
            CapEngine::detectMBRCollisionInterior(newPlayerObject->boundingPolygon(), windowRect);

        if (collisionType == CapEngine::CollisionType::COLLISION_NONE) {
            m_playerObject = std::move(newPlayerObject);
        }

        // check ball and paddle collision

        // check ball and block collisions

        // check ball and wall collisions

        // check ball a bottom collisions
    };

    auto handleWinState = [&]() {
        if (m_gameState.inTransition) {
            m_timing.transitionTimeMs += timestepMs;
        }

        const double requiredTransitionTimeRequired = 2000.0;
        if (m_timing.transitionTimeMs > requiredTransitionTimeRequired) {
            m_timing.transitionTimeMs = 0.0;
            m_gameState.inTransition = false;
        }
    };

    auto handleDeadState = [&]() { nullptr; };

    switch (m_gameState.status) {
        case GameStatus::Starting:
            handleStartingState();
            break;
        case GameStatus::Active:
            handleActiveState();
            break;
        case GameStatus::Win:
            handleWinState();
            break;
        case GameStatus::Dead:
            handleDeadState();
            break;
    }
}

//! Handles keyboard events.
/**
 \param in_event
   The keyboard event.
*/
void MainGameState::handleKeyboardEvent(const SDL_KeyboardEvent& in_event)
{
    // if (m_gameState.status == GameStatus::Active) {
    //     if (in_event.type == SDL_KEYUP && in_event.keysym.sym == SDLK_SPACE) {
    //         PlayerInputEvent playerInputEvent;
    //         playerInputEvent.inputType = PlayerInputEvent::PlayerInputType::Jump;
    //         CapEngine::Locator::getEventSubscriber().m_gameEventSignal(playerInputEvent);
    //     }
    // }

    // if (m_gameState.status == GameStatus::Dead || m_gameState.status == GameStatus::Win && !m_gameState.inTransition)
    // {
    //     if (in_event.type == SDL_KEYUP && in_event.keysym.sym == SDLK_SPACE) {
    //         m_gameState.status = GameStatus::Starting;
    //         m_telemetry.elapsedTimeMs = 0.0;
    //         m_cats.clear();
    //         m_playerObject = createPlayerObject(m_windowId);
    //     }
    // }
}

}  // namespace Breakout
