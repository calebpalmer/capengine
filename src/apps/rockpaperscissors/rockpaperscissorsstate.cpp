#include "rockpaperscissorsstate.h"

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include <capengine/collision.h>
#include <capengine/colour.h>
#include <capengine/filesystem.h>
#include <capengine/locator.h>
#include <capengine/tiledmap.h>
#include <capengine/tiledobjectgroup.h>
#include <capengine/tiledtileset.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <random>

namespace RPS {

namespace {

//! the amount of milliseconds the player has to make a choice
const int kChoiceTimeMs = 3000;
//! The amount of milliseconds the game waits after the player has made a choice
const int kEndChoiceWaitTimeMs = 2000;

constexpr int kNumTilesets = 2;
constexpr int kNumObjectGroups = 2;

Choice generateRandomChoice()
{
    std::random_device randomDevice;
    std::default_random_engine randomEngine(randomDevice());

    std::uniform_int_distribution<int> uniformDist(1, 3);

    return static_cast<Choice>(uniformDist(randomEngine));
}

CapEngine::Rect getTilePosition(CapEngine::TiledTileset const& in_tileset, RPS::Choice in_choice)
{
    switch (in_choice) {
        case RPS::Choice::Rock:
            return in_tileset.tileRect(0).toRect();
        case RPS::Choice::Paper:
            return in_tileset.tileRect(1).toRect();
        case RPS::Choice::Scissors:
            return in_tileset.tileRect(2).toRect();
        default:
            return in_tileset.tileRect(0).toRect();
    }
}

}  // namespace

State::State() : settings{1}, phase(GamePhase::Start) {}

RockPaperScissorsState::RockPaperScissorsState(uint32_t in_windowId) : m_windowId(in_windowId)
{
    // read in tileset from rps_resources/rps.tsj
    std::filesystem::path tiledMapPath =
        std::filesystem::path{
            std::filesystem::path{CapEngine::getCurrentExecutablePath()}.parent_path().parent_path()} /
        "rps_resources" / "map.tmj";
    std::ifstream mapIn{tiledMapPath};
    jsoncons::json mapJson = jsoncons::json::parse(mapIn);
    m_map = std::make_unique<CapEngine::TiledMap>(mapJson, tiledMapPath);
    assert(m_map->tilesets().size() >= 2);

    auto& videoManager = CapEngine::Locator::getVideoManager();
    auto [windowWidth, windowHeight] = videoManager.getWindowResolution(m_windowId);
    videoManager.setWindowLogicalResolution(m_windowId, m_map->width() * m_map->tileWidth(),
                                            m_map->height() * m_map->tileHeight());

    // set default keybindings
    m_state.settings.globalKeybindings.startKeycode = SDLK_SPACE;
    m_state.settings.globalKeybindings.exitKeycode = SDLK_ESCAPE;
    m_state.settings.player1Keybindings = PlayerKeybindings{SDLK_a, SDLK_s, SDLK_d};
    m_state.settings.player2Keybindings = PlayerKeybindings{SDLK_LEFT, SDLK_DOWN, SDLK_RIGHT};

    // register for keyboard events
    CapEngine::Locator::getEventSubscriber().m_keyboardEventSignal.connect(
        [this](SDL_KeyboardEvent in_event) { this->handleKeyboardEvent(in_event); });
}

void RockPaperScissorsState::renderPlayers()
{
    auto& videoManager = CapEngine::Locator::getVideoManager();
    assert(m_map != nullptr);

    auto const& tilesets = m_map->tilesets();
    assert(tilesets.size() >= kNumTilesets);

    // get the tileset with the hands
    auto const& tileset = std::find_if(tilesets.begin(), tilesets.end(),
                                       [](auto const& tileset) { return tileset->name() == "rps.tsj"; });
    assert(tileset != tilesets.end());
    auto&& texture = (*tileset)->texture();
    assert(texture != std::nullopt);

    auto const& objectGroups = m_map->objectGroups();
    assert(objectGroups.size() == kNumObjectGroups);
    auto const& objectGroup = objectGroups[0];

    videoManager.drawTexture(m_windowId, m_state.player1.drawPosition, *texture, &m_state.player1.tilePosition);
    videoManager.drawTexture(m_windowId, *texture, &m_state.player2.tilePosition, &m_state.player2.drawPosition,
                             std::nullopt, SDL_FLIP_HORIZONTAL);
}

void RockPaperScissorsState::renderScore() {}

void RockPaperScissorsState::render()
{
    auto& videoManager = CapEngine::Locator::getVideoManager();
    auto [windowWidth, windowHeight] = videoManager.getWindowResolution(m_windowId);

    auto [logicalWidth, logicalHeight] = videoManager.getWindowLogicalResolution(m_windowId);

    // render background first
    videoManager.drawFillRect(m_windowId, CapEngine::Rect{0, 0, logicalWidth, logicalHeight},
                              CapEngine::Colour{0xA0, 0xA0, 0xA0, 0xFF});

    // render the map and draw it to the window
    assert(m_map != nullptr);
    m_map->render();
    videoManager.drawTexture(m_windowId,
                             SDL_Rect{0, 0, m_map->width() * m_map->tileWidth(), m_map->height() * m_map->tileHeight()},
                             m_map->texture());

    renderPlayers();
    renderScore();
}

void RockPaperScissorsState::update(double ms)
{
    // The game is not in progress
    if (m_state.phase == GamePhase::Start || m_state.phase == GamePhase::Stopped) {
        this->updateStopped(ms);
    }

    // In choice state
    else if (m_state.phase == GamePhase::Choice || m_state.phase == GamePhase::ChoiceEnd) {
        this->updateChoose(ms);
    }

    // exit the game
    else {
        // TODO
    }
}

void RockPaperScissorsState::updateStopped(double in_ms)
{
    assert(m_map != nullptr);

    auto const& tilesets = m_map->tilesets();
    assert(tilesets.size() >= kNumTilesets);
    // get the tileset with the hands
    auto const& tileset = std::find_if(tilesets.begin(), tilesets.end(),
                                       [](auto const& tileset) { return tileset->name() == "rps.tsj"; });
    assert(tileset != tilesets.end());

    auto const& objectGroups = m_map->objectGroups();
    assert(objectGroups.size() == kNumObjectGroups);
    auto const& objectGroup = objectGroups[0];

    // update left side
    auto maybeLeftObject = objectGroup.objectByName("player-1-start");
    assert(maybeLeftObject != std::nullopt);
    assert(maybeLeftObject.has_value());

    // the location in the tileset of the rock position
    m_state.player1.tilePosition = (*tileset)->tileRect(0).toRect();

    // the location in the window to draw the rock
    m_state.player1.drawPosition = {static_cast<int>(maybeLeftObject->x), static_cast<int>(maybeLeftObject->y),
                                    (*tileset)->tileWidth(), (*tileset)->tileHeight()};

    // update right side
    auto maybeRightObject = objectGroup.objectByName("player-2-start");
    assert(maybeRightObject != std::nullopt);
    assert(maybeRightObject.has_value());

    // the location in the tileset of the rock position
    m_state.player2.tilePosition = (*tileset)->tileRect(0).toRect();

    // the location in the window to draw the rock
    m_state.player2.drawPosition =
        CapEngine::Rect{static_cast<int>(maybeRightObject->x), static_cast<int>(maybeRightObject->y),
                        (*tileset)->tileWidth(), (*tileset)->tileHeight()};
}

void RockPaperScissorsState::updateChoose(double in_ms)
{
    // grab a bunch of stuff that we need
    assert(m_map != nullptr);

    auto const& tilesets = m_map->tilesets();
    assert(tilesets.size() >= 2);

    auto const& tileset = std::find_if(tilesets.begin(), tilesets.end(),
                                       [](auto const& tileset) { return tileset->name() == "rps.tsj"; });
    assert(tileset != tilesets.end());

    auto const& objectGroups = m_map->objectGroups();
    assert(objectGroups.size() == kNumObjectGroups);
    auto const& objectGroup = objectGroups[0];

    auto leftObjectStart = objectGroup.objectByName("player-1-start");
    assert(leftObjectStart != std::nullopt);

    auto rightObjectStart = objectGroup.objectByName("player-2-start");
    assert(rightObjectStart != std::nullopt);

    auto leftObjectEnd = objectGroup.objectByName("player-1-end");
    assert(leftObjectEnd != std::nullopt);

    auto rightObjectEnd = objectGroup.objectByName("player-2-end");
    assert(rightObjectEnd != std::nullopt);

    // increment our current timer in this state
    m_state.choiceTimeMs += in_ms;

    // if were still under choide time, perform the movement
    if (m_state.choiceTimeMs < kChoiceTimeMs) {
        int timePerMovementMs = kChoiceTimeMs / 5;

        int currentMovement = m_state.choiceTimeMs / timePerMovementMs;
        int currentMovementTime = m_state.choiceTimeMs % timePerMovementMs;

        double velocity = (leftObjectEnd->y - leftObjectStart->y) / timePerMovementMs;
        int displacement = velocity * static_cast<double>(currentMovementTime);

        if (currentMovement % 2 == 0) {
            // player 1
            m_state.player1.drawPosition.y = leftObjectStart->y + displacement;
            m_state.player1.drawPosition.x = leftObjectStart->x;
            // player 2
            m_state.player2.drawPosition.y = rightObjectStart->y + displacement;
            m_state.player2.drawPosition.x = rightObjectStart->x;
        }
        else {
            // calculate location based off end position
            // player 1
            m_state.player1.drawPosition.y = leftObjectEnd->y - displacement;
            m_state.player1.drawPosition.x = leftObjectEnd->x;
            // player 2
            m_state.player2.drawPosition.y = rightObjectEnd->y - displacement;
            m_state.player2.drawPosition.x = rightObjectEnd->x;
        }
    }
    // choice time is over and we've already waited a bit after so switch to
    // stopped state
    else if (m_state.choiceTimeMs > kChoiceTimeMs + kEndChoiceWaitTimeMs) {
        m_state.choiceTimeMs = 0;
        m_state.phase = GamePhase::Stopped;
        m_state.player1.currentChoice = Choice::Rock;
        m_state.player2.currentChoice = Choice::Rock;
        m_state.player1.nextChoice = std::nullopt;
        m_state.player2.nextChoice = std::nullopt;
    }
    // choice time is over but wait a bit to show the end choices
    else {
        if (m_state.phase == GamePhase::Choice) {
            m_state.phase = GamePhase::ChoiceEnd;

            m_state.player1.drawPosition.y = leftObjectEnd->y;
            m_state.player1.drawPosition.x = leftObjectEnd->x;
            m_state.player2.drawPosition.y = rightObjectEnd->y;
            m_state.player2.drawPosition.x = rightObjectEnd->x;

            if (m_state.player1.nextChoice == std::nullopt) {
                m_state.player1.nextChoice = Choice::Rock;
            }

            if (m_state.player2.nextChoice == std::nullopt) {
                m_state.player2.nextChoice = Choice::Rock;
            }

            m_state.player1.currentChoice = *m_state.player1.nextChoice;
            m_state.player2.currentChoice = *m_state.player2.nextChoice;

            m_state.player1.tilePosition = getTilePosition(**tileset, m_state.player1.currentChoice);
            m_state.player2.tilePosition = getTilePosition(**tileset, m_state.player2.currentChoice);

            // calculate the winner
            if (m_state.player1.currentChoice == Choice::Rock && m_state.player2.currentChoice == Choice::Scissors) {
                m_state.player1.score += 1;
            }
            else if (m_state.player1.currentChoice == Choice::Rock && m_state.player2.currentChoice == Choice::Paper) {
                m_state.player2.score += 1;
            }
            else if (m_state.player1.currentChoice == Choice::Paper && m_state.player2.currentChoice == Choice::Rock) {
                m_state.player1.score += 1;
            }
            else if (m_state.player1.currentChoice == Choice::Paper &&
                     m_state.player2.currentChoice == Choice::Scissors) {
                m_state.player2.score += 1;
            }
            else if (m_state.player1.currentChoice == Choice::Scissors &&
                     m_state.player2.currentChoice == Choice::Paper) {
                m_state.player1.score += 1;
            }
            else if (m_state.player1.currentChoice == Choice::Scissors &&
                     m_state.player2.currentChoice == Choice::Rock) {
                m_state.player2.score += 1;
            }

            // else no one wins

            std::cout << "Player 1: " << m_state.player1.score << " Player 2: " << m_state.player2.score << std::endl;
        }
    }
}

void RockPaperScissorsState::handleKeyboardEvent(SDL_KeyboardEvent in_event)
{
    if (m_state.phase == GamePhase::Start || m_state.phase == GamePhase::Stopped) {
        if (in_event.type == SDL_KEYUP && in_event.keysym.sym == m_state.settings.globalKeybindings.startKeycode) {
            m_state.phase = GamePhase::Choice;
        }
    }

    else if (m_state.phase == GamePhase::Choice) {
        if (in_event.type == SDL_KEYUP) {
            if (in_event.keysym.sym == m_state.settings.player1Keybindings.rockKeycode &&
                m_state.player1.nextChoice == std::nullopt) {
                m_state.player1.nextChoice = Choice::Rock;
            }
            else if (in_event.keysym.sym == m_state.settings.player1Keybindings.paperKeycode &&
                     m_state.player1.nextChoice == std::nullopt) {
                m_state.player1.nextChoice = Choice::Paper;
            }
            else if (in_event.keysym.sym == m_state.settings.player1Keybindings.scissorsKeycode &&
                     m_state.player1.nextChoice == std::nullopt) {
                m_state.player1.nextChoice = Choice::Scissors;
            }
            else if (in_event.keysym.sym == m_state.settings.player2Keybindings.rockKeycode &&
                     m_state.player2.nextChoice == std::nullopt) {
                m_state.player2.nextChoice = Choice::Rock;
            }
            else if (in_event.keysym.sym == m_state.settings.player2Keybindings.paperKeycode &&
                     m_state.player2.nextChoice == std::nullopt) {
                m_state.player2.nextChoice = Choice::Paper;
            }
            else if (in_event.keysym.sym == m_state.settings.player2Keybindings.scissorsKeycode &&
                     m_state.player2.nextChoice == std::nullopt) {
                m_state.player2.nextChoice = Choice::Scissors;
            }
        }
    }

    else {
        //
    }
}

}  // namespace RPS
