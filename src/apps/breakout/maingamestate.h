#ifndef FLAPPYPEI_FLAPPYPEISTATE_H
#define FLAPPYPEI_FLAPPYPEISTATE_H

#include <SDL2/SDL.h>
#include <capengine/camera2d.h>
#include <capengine/gameobject.h>
#include <capengine/gamestate.h>

#include <cstdint>
#include <gsl/gsl-lite.hpp>
#include <memory>

namespace Breakout {

//! The main game state for Flappy Pei.
class MainGameState final : public CapEngine::GameState {
   public:
    //! The current game status
    enum class GameStatus {
        Starting,  //!< The game is starting.
        Active,    //!< The game is currently being played.
        Dead,      //!< The player has died.
        Win        //!< The player has won.
    };

    //! The current game state.
    struct GameState {
        GameStatus status = GameStatus::Starting;  //!< The current game status.
        bool inTransition = false;
    };

    //! Telemetry data for the game state.
    struct Timing {
        double elapsedTimeMs = 0.0;  //!< The elapsed time in milliseconds.
        double currentLevelTimeMs = 0.0;
        double transitionTimeMs = 0.0;
    };

    struct LevelSettings {
        double totalTime;
        int velocity;
        int gapSize;
        int catInterval;
    };

    explicit MainGameState(uint32_t in_windowId);
    ~MainGameState() override = default;

    void render() override;
    void update(double timestepMs) override;
    void handleKeyboardEvent(const SDL_KeyboardEvent& event);

   private:
    void generateCats();

    uint32_t m_windowId;                                                 //!< The id of the window.
    Timing m_timing;                                                     //!< The telemetry data for the game state.
    GameState m_gameState;                                               //!< The current game state.
    CapEngine::Camera2d m_camera;                                        //!< The camera used for rendering.
    std::unique_ptr<CapEngine::GameObject> m_playerObject;               //!< The player object.
    std::unique_ptr<CapEngine::GameObject> m_ballObject;                 //!< The player object.
    std::vector<std::unique_ptr<CapEngine::GameObject>> m_blockObjects;  //!< The player object.
};

}  // namespace Breakout

#endif // FLAPPYPEI_FLAPPYPEISTATE_H
