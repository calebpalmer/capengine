#include "maingamestate.h"

#include <SDL_events.h>
#include <apps/flappypei/catgraphicscomponent.h>
#include <apps/flappypei/gameevents.h>
#include <capengine/CapEngineException.h>
#include <capengine/collision.h>
#include <capengine/colour.h>
#include <capengine/components.h>
#include <capengine/gameobject.h>
#include <capengine/gamestate.h>
#include <capengine/locator.h>
#include <capengine/logger.h>
#include <capengine/matrix.h>
#include <capengine/vector.h>
#include <capengine/logging.h>
#include <capengine/boxcollider.h>

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <boost/throw_exception.hpp>
#include <cstdint>
#include <memory>
#include <random>
#include <ranges>
#include <string>
#include <algorithm>
#include <vector>

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
    const double paddleY = 10;
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

    CapEngine::Vector initialPosition{(kLogicalWindowWidth / 2.0) - (kBallDiameter / 2.0),
                                      (kLogicalWindowHeight / 2.0) - (kBallDiameter / 2.0)};
    ballObject->setPosition(initialPosition);
    // set the initial velocity vector randomly in the downward directiion
    double angle = generateRandomNumber(225, 315);
    CapEngine::PolarVector polarVector{kBallVelocity, angle};
    ballObject->setVelocity(CapEngine::Vector{polarVector});

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
    blockObject->addComponent(std::make_shared<CapEngine::BoxCollider>(
        CapEngine::Rectangle{in_position.getX(), in_position.getY(), static_cast<double>(in_blockWidth), kBlockHeight},
        CapEngine::Anchor::BottomLeft));

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
            CapEngine::Vector position{
                (col * blockWidth) + (col * kGap) + kGap,
                kLogicalWindowHeight - kGap - kBlockHeight - ((row * kBlockHeight) + (row * kGap))};
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

    auto handleDead = [&]() { renderBanner(m_windowId, "You suck!"); };

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
        // update objects
        assert(m_playerObject != nullptr);
        m_playerObject->updateInPlace(timestepMs);

        assert(m_ballObject != nullptr);
        m_ballObject->updateInPlace(timestepMs);

        for (auto&& block : m_blockObjects) {
            CAP_THROW_NULL(block);
            block->updateInPlace(timestepMs);
        }

        // do collision detections
        const CapEngine::Rectangle windowRect{0, 0, kLogicalWindowWidth, kLogicalWindowHeight};
        // paddle collision with window
        {
            const CapEngine::CollisionType collisionType =
                CapEngine::detectMBRCollisionInterior(m_playerObject->boundingPolygon(), windowRect);

            // handle collisions
            // out of bounds collision detection
            if (collisionType != CapEngine::CollisionType::COLLISION_NONE)
                m_playerObject->setPosition(m_playerObject->getPreviousPosition());
        }

        // check ball and paddle collision
        {
            auto boxCollision =
                CapEngine::detectBoxCollision(m_playerObject->boundingPolygon(), m_ballObject->boundingPolygon(),
                                              CapEngine::RepresentativePointMethod::Simple);
            if (boxCollision) {
                if (boxCollision->collisionType == CapEngine::CollisionType::COLLISION_TOP ||
                    boxCollision->collisionType == CapEngine::CollisionType::COLLISION_BOTTOM) {
                    auto ballVelocity = m_ballObject->getVelocity();
                    ballVelocity.setY(ballVelocity.getY() * (-1.0));

                    // change x?
                    auto paddleRect = m_playerObject->boundingPolygon();
                    auto ballRect = m_ballObject->boundingPolygon();

                    auto paddleCenter = paddleRect.x + (paddleRect.width / 2.0);
                    auto paddleLeft = paddleCenter - (paddleRect.width / 2.0 / 2.0);
                    auto paddleRight = paddleCenter + (paddleRect.width / 2.0 / 2.0);
                    auto ballCenter = ballRect.x + (ballRect.width / 2.0);

                    std::optional<CapEngine::Matrix> rotationMatrix;
                    if (ballCenter < paddleLeft) {
                        rotationMatrix = CapEngine::Matrix::createZRotationMatrix(kBallFarAngleDegrees);
                    }
                    else if (ballCenter > paddleRight) {
                        rotationMatrix = CapEngine::Matrix::createZRotationMatrix((-1) * kBallFarAngleDegrees);
                    }

                    if (rotationMatrix) {
                        ballVelocity = *rotationMatrix * ballVelocity;

                        // make sure the angle doesn't get too much
                        auto polarVector = ballVelocity.toPolar();
                        if (polarVector.deg > 145) {
                            polarVector.deg = 145;
                            ballVelocity = CapEngine::Vector{polarVector};
                        }
                        if (polarVector.deg < 45) {
                            polarVector.deg = 45;
                            ballVelocity = CapEngine::Vector{polarVector};
                        }
                    }
                    m_ballObject->setVelocity(ballVelocity);

                    // play collision sound
                    try {
                        CapEngine::Locator::getAssetManager().playSound(kCollisionSound);
                    }
                    catch (const CapEngine::CapEngineException& e) {
                        CapEngine::logException(e);
                    }
                }
            }
        }

        // check ball and block collisions
        for (auto&& block : m_blockObjects) {
            CAP_THROW_NULL(block);
            auto boxCollision =
                CapEngine::detectBoxCollision(block->boundingPolygon(), m_ballObject->boundingPolygon());

            if (boxCollision && boxCollision->collisionType != CapEngine::CollisionType::COLLISION_NONE) {
                // destroy the block
                block->setObjectState(CapEngine::GameObject::ObjectState::Dead);

                // change the balls vector
                auto ballVelocity = m_ballObject->getVelocity();
                if (boxCollision->collisionType == CapEngine::CollisionType::COLLISION_BOTTOM ||
                    boxCollision->collisionType == CapEngine::CollisionType::COLLISION_TOP) {
                    ballVelocity.setY(ballVelocity.getY() * (-1.0));
                }
                else if (boxCollision->collisionType == CapEngine::CollisionType::COLLISION_LEFT ||
                         boxCollision->collisionType == CapEngine::CollisionType::COLLISION_RIGHT) {
                    ballVelocity.setX(ballVelocity.getX() * (-1.0));
                }
                m_ballObject->setVelocity(ballVelocity);

                // play collision sound
                try {
                    CapEngine::Locator::getAssetManager().playSound(kBlockCollisionSound);
                }
                catch (const CapEngine::CapEngineException& e) {
                    CapEngine::logException(e);
                }
            }
        }

        // check ball and wall collisions
        {
            const CapEngine::CollisionType collisionType =
                CapEngine::detectMBRCollisionInterior(m_ballObject->boundingPolygon(), windowRect);

            if (collisionType != CapEngine::CollisionType::COLLISION_NONE) {
                CapEngine::Vector velocity = m_ballObject->getVelocity();
                switch (collisionType) {
                    case CapEngine::CollisionType::COLLISION_LEFT:
                    case CapEngine::CollisionType::COLLISION_RIGHT:
                        velocity.setX(velocity.getX() * (-1.0));
                        break;
                    case CapEngine::CollisionType::COLLISION_BOTTOM:
                        m_gameState.status = GameStatus::Dead;
                    case CapEngine::CollisionType::COLLISION_TOP:
                        velocity.setY(velocity.getY() * (-1.0));
                        break;
                    case CapEngine::CollisionType::COLLISION_GENERAL:
                    case CapEngine::CollisionType::COLLISION_ONX:
                    case CapEngine::CollisionType::COLLISION_ONY:
                    case CapEngine::CollisionType::COLLISION_NONE:
                    default:
                        BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning)
                            << "Unexpected collision detected betwen ball and wall.";
                        break;
                }
                m_ballObject->setVelocity(velocity);

                // play collision sound
                try {
                    CapEngine::Locator::getAssetManager().playSound(kCollisionSound);
                }
                catch (const CapEngine::CapEngineException& e) {
                    CapEngine::logException(e);
                }
            }
        }

        // clean up objects
        std::erase_if(m_blockObjects, [](const std::unique_ptr<CapEngine::GameObject>& block) {
            return block->getObjectState() == CapEngine::GameObject::ObjectState::Dead;
        });

        if (m_blockObjects.size() == 0) {
            m_gameState.status = GameStatus::Win;
        }
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
