#include "maingamestate.h"

#include <SDL_events.h>
#include <apps/flappypei/catgraphicscomponent.h>
#include <apps/flappypei/gameevents.h>
#include <capengine/CapEngineException.h>
#include <capengine/collision.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>

#include <algorithm>
#include <memory>
#include <random>
#include <string>

#include "catgraphicscomponent.h"
#include "catphysicscomponent.h"
#include "constants.h"
#include "playergraphicscomponent.h"
#include "playerphysicscomponent.h"

namespace FlappyPei {

namespace {

int generateRandomNumber(int in_min, int in_max)
{
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());
    std::uniform_int_distribution<int> uniformDist(in_min, in_max);

    return uniformDist(randomEngine);
}

std::unique_ptr<CapEngine::GameObject> createPlayerObject(uint32_t in_windowId)
{
    auto playerObject = std::make_unique<CapEngine::GameObject>();
    playerObject->addComponent(std::make_shared<FlappyPei::PlayerGraphicsComponent>());
    playerObject->addComponent(std::make_shared<FlappyPei::PlayerPhysicsComponent>());

    CapEngine::Vector initialPosition{(kLogicalWindowWidth / 2) + (kSpriteWidth / 2),
                                      (kLogicalWindowHeight / 2) + (kSpriteHeight / 2)};
    playerObject->setPosition(initialPosition);  // Set initial position
    playerObject->setObjectState(CapEngine::GameObject::ObjectState::Starting);

    return playerObject;
}

gsl::not_null<std::unique_ptr<CapEngine::GameObject>> createCatObject(uint32_t in_windowId, int in_gapSize,
                                                                      int in_previousGapSize, int in_previousGapStart,
                                                                      int in_maxLocationDifference, int in_velocity,
                                                                      CapEngine::Vector const& in_initialPosition)
{
    auto catObject = std::make_unique<CapEngine::GameObject>();

    // find min y and max
    int minGapLocation = in_previousGapStart - in_maxLocationDifference - in_gapSize;
    int maxGapLocation = in_previousGapStart + in_previousGapSize + in_maxLocationDifference;

    if (minGapLocation < 0) {
        minGapLocation = 0;
    }

    if (maxGapLocation + in_gapSize > kLogicalWindowHeight) {
        maxGapLocation = kLogicalWindowHeight - in_gapSize;
    }

    // generate a random number between minX and maxX
    int gapLocation = generateRandomNumber(minGapLocation, maxGapLocation);

    catObject->addComponent(std::make_shared<FlappyPei::CatGraphicsComponent>(gapLocation, in_gapSize));
    catObject->addComponent(std::make_shared<FlappyPei::CatPhysicsComponent>(gapLocation, in_gapSize));

    catObject->setPosition(in_initialPosition);  // Set initial position
    catObject->setVelocity(CapEngine::Vector{in_velocity});
    catObject->setObjectState(CapEngine::GameObject::ObjectState::Active);

    return catObject;
}

std::queue<MainGameState::LevelSettings> makeLevels()
{
    std::queue<MainGameState::LevelSettings> levels;
    levels.push(MainGameState::LevelSettings{.totalTime = 30.0 * 1000.0,
                                             .velocity = kStartingCatVelocity,
                                             .gapSize = kStartingGapSize,
                                             .catInterval = kStartingCatInterval});

    return levels;
}

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

}  // namespace

/**
 * \brief Constructor
 * \param in_windowId The window id.
 */
MainGameState::MainGameState(uint32_t in_windowId)
    : CapEngine::GameState(),
      m_windowId(in_windowId),
      m_camera({kLogicalWindowHeight, kLogicalWindowWidth}),
      m_playerObject(createPlayerObject(m_windowId)),
      m_levels(makeLevels())
{
    // register for keyboard events
    CapEngine::Locator::getEventSubscriber().m_keyboardEventSignal.connect(
        [this](SDL_KeyboardEvent in_event) { this->handleKeyboardEvent(in_event); });
}

/**
 * \brief Renders the game state.
 */
void MainGameState::render()
{
    // render player
    m_playerObject->render(m_camera, m_windowId);

    // render cats
    std::ranges::for_each(m_cats, [&](const auto& cat) { cat->render(m_camera, m_windowId); });

    if (m_gameState.status == GameStatus::Starting) {
        // display start timer
        std::string renderText =
            std::to_string(static_cast<int>(3 - static_cast<int>((m_telemetry.elapsedTimeMs / 1000.0))));

        renderBanner(m_windowId, renderText);
    }

    if (m_gameState.status == GameStatus::Dead) {
        renderBanner(m_windowId, "Game Over!");
    }

    if (m_gameState.status == GameStatus::Win) {
        renderBanner(m_windowId, "You Win!");
    }
}

/**
 * \brief Updates the game state.
 * \param timestepMs The timestep in milliseconds.
 */
void MainGameState::update(double timestepMs)
{
    m_telemetry.elapsedTimeMs += timestepMs;
    m_telemetry.currentLevelTimeMs += timestepMs;

    // game starting
    if (m_gameState.status == GameStatus::Starting && m_telemetry.elapsedTimeMs >= kGameWaitTime) {
        // set the game status
        m_gameState.status = GameStatus::Active;

        // start the player
        if (m_playerObject->getObjectState() == CapEngine::GameObject::ObjectState::Starting) {
            m_playerObject->setObjectState(CapEngine::GameObject::ObjectState::Active);
        }
    }

    // game active
    if (m_gameState.status == GameStatus::Active) {
        // update cats
        decltype(m_cats) newCats;
        std::ranges::for_each(m_cats, [&](const auto& cat) { newCats.push_back(cat->update(timestepMs)); });
        m_cats = std::move(newCats);

        // update player
        std::unique_ptr<CapEngine::GameObject> newPlayerObject = m_playerObject->update(timestepMs);
        CAP_THROW_NULL(newPlayerObject, "New player object is null");

        // collision with window
        const CapEngine::Rectangle windowRect{0, 0, kLogicalWindowWidth, kLogicalWindowHeight};
        const CapEngine::CollisionType collisionType =
            CapEngine::detectMBRCollisionInterior(newPlayerObject->boundingPolygon(), windowRect);

        if (collisionType == CapEngine::CollisionType::COLLISION_NONE) {
            m_playerObject = std::move(newPlayerObject);
        }

        // collisions with cats
        bool collidesWithCat = std::ranges::any_of(m_cats, [&](auto const& in_cat) {
            // When calling a template member function on a dependent type, you must prefix the call with the template
            // keyword to tell the compiler it's a template.
            std::vector<std::shared_ptr<CatPhysicsComponent>> physicsCompoinents =
                in_cat->template getComponents<CatPhysicsComponent>();
            assert(physicsCompoinents.size() == 1);

            gsl::not_null<std::shared_ptr<CatPhysicsComponent>> const& physicsComponent = physicsCompoinents[0];

            return physicsComponent->collides(m_playerObject->boundingPolygon()) !=
                   CapEngine::CollisionType::COLLISION_NONE;
        });

        if (collidesWithCat) {
            m_gameState.status = GameStatus::Dead;
            std::cout << "Cat got you!\n";
        }

        // check level
        if (m_telemetry.currentLevelTimeMs > m_levels.front().totalTime) {
            m_levels.pop();
            m_telemetry.currentLevelTimeMs = 0.0;
            if (m_levels.size() == 0) {
                m_gameState.status = GameStatus::Win;
            }
        }

        // generate new cats if not dead
        generateCats();
    }
}

/**
 * \brief Handles keyboard events.
 * \param event The keyboard event.
 */
void MainGameState::handleKeyboardEvent(const SDL_KeyboardEvent& in_event)
{
    if (m_gameState.status == GameStatus::Active) {
        if (in_event.type == SDL_KEYUP && in_event.keysym.sym == SDLK_SPACE) {
            PlayerInputEvent playerInputEvent;
            playerInputEvent.inputType = PlayerInputEvent::PlayerInputType::Jump;
            CapEngine::Locator::getEventSubscriber().m_gameEventSignal(playerInputEvent);
        }
    }

    if (m_gameState.status == GameStatus::Dead || m_gameState.status == GameStatus::Win) {
        if (in_event.type == SDL_KEYUP && in_event.keysym.sym == SDLK_SPACE) {
            m_gameState.status = GameStatus::Starting;
            m_telemetry.elapsedTimeMs = 0.0;
            m_cats.clear();
            m_playerObject = createPlayerObject(m_windowId);
        }
    }
}

void MainGameState::generateCats()
{
    CapEngine::Vector position{1600};

    LevelSettings const& currentLevelSettings = m_levels.front();

    int previousGapSize = currentLevelSettings.gapSize;
    int previousGapStart = 450;  // just make a starting point. It'll get updated when we retrieve the last cat.

    // get the last cat
    std::optional<CapEngine::Rectangle> previousMbr;

    if (m_cats.size() > 0) {
        gsl::not_null<std::unique_ptr<CapEngine::GameObject>> const& lastCat = m_cats.back();

        std::vector<std::shared_ptr<CatPhysicsComponent>> physicsCompoinents =
            lastCat->getComponents<CatPhysicsComponent>();
        assert(physicsCompoinents.size() == 1);

        gsl::not_null<std::shared_ptr<CatPhysicsComponent>> const& physicsComponent = physicsCompoinents[0];
        previousMbr = physicsComponent->boundingPolygon(*lastCat);
        assert(previousMbr.has_value());

        previousGapSize = physicsComponent->gapSize();
        previousGapStart = physicsComponent->gapLocation();
    }

    // if this is the first cat or the last cat is fully in the screen
    if (previousMbr == std::nullopt || (previousMbr->x + previousMbr->width) <= kLogicalWindowWidth) {
        m_cats.push_back(createCatObject(m_windowId, currentLevelSettings.gapSize, previousGapSize, previousGapStart,
                                         currentLevelSettings.catInterval, currentLevelSettings.velocity, position));
    }
}

}  // namespace FlappyPei
