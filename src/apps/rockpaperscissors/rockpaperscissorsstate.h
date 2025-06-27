#ifndef RPS_ROCKPAPERSCISSORSSTATE_H
#define RPS_ROCKPAPERSCISSORSSTATE_H

#include <capengine/captypes.h>
#include <capengine/gamestate.h>
#include <capengine/tiledmap.h>
#include <capengine/tiledtileset.h>

#include <cstdint>
#include <SDL2/SDL.h>

namespace RPS
{

//! Game phase enumeration
/**
 * \brief Represents the different phases of the Rock Paper Scissors game.
 */
enum class GamePhase { Start, Stopped, Choice, ChoiceEnd, Exit };

//! Player choice enumeration
/**
 * \brief Represents the possible choices a player can make.
 */
enum class Choice { Rock = 0, Paper, Scissors };

//! Player key bindings
/**
 * \brief Defines the keyboard controls for a single player.
 */
struct PlayerKeybindings {
    SDL_Keycode rockKeycode;     //!< Key to select Rock
    SDL_Keycode paperKeycode;    //!< Key to select Paper
    SDL_Keycode scissorsKeycode; //!< Key to select Scissors
};
//! Global key bindings
/**
 * \brief Defines the global keyboard controls for game management.
 */
struct GlobalKeybindings {
    SDL_Keycode startKeycode; //!< Key to start a new round
    SDL_Keycode exitKeycode;  //!< Key to exit the game
};

//! Game settings
/**
 * \brief Contains all configurable game settings including key bindings.
 */
struct Settings {
    int numPlayers = 1;                       //!< Number of players (currently supports 1 or 2)
    GlobalKeybindings globalKeybindings;      //!< Global game control keys
    PlayerKeybindings player1Keybindings;     //!< Player 1's control keys
    PlayerKeybindings player2Keybindings;     //!< Player 2's control keys
};

//! Player state
/**
 * \brief Represents the current state of a single player.
 */
struct PlayerState {
    CapEngine::Rect tilePosition;                     //!< Position in the tileset for current choice sprite
    CapEngine::Rect drawPosition;                     //!< Position on screen where player sprite is drawn
    Choice currentChoice = Choice::Rock;              //!< The player's currently displayed choice
    std::optional<Choice> nextChoice = std::nullopt; //!< The player's next choice (during selection phase)
    int score = 0;                                    //!< The player's current score
};

//! Game state
/**
 * \brief Contains the complete state of the Rock Paper Scissors game.
 */
struct State {
    State();
    Settings settings;                       //!< Game configuration settings
    GamePhase phase = GamePhase::Start;      //!< Current phase of the game
    PlayerState player1;                     //!< State of player 1
    PlayerState player2;                     //!< State of player 2
    int choiceTimeMs = 0;                    //!< Time elapsed in current choice phase (milliseconds)
    int currentWinner = 0;                   //!< Winner of the current round (1 or 2, 0 for tie)
};

//! Rock Paper Scissors game state
/**
 * \brief Main game state class that handles the Rock Paper Scissors game logic and rendering.
 * 
 * This class manages a complete Rock Paper Scissors game including player input,
 * animation timing, score tracking, and victory conditions. The game supports
 * two players with configurable key bindings.
 */
class RockPaperScissorsState final : public CapEngine::GameState {
   public:
    explicit RockPaperScissorsState(uint32_t in_windowId);
    ~RockPaperScissorsState() override = default;

    void render() override;
    void update(double ms) override;

   private:
    void renderPlayers();
    void renderScore();
    void renderVictoryBanner();
    void handleKeyboardEvent(SDL_KeyboardEvent in_event);

    // State update functions
    void updateStopped(double ms);
    void updateChoose(double ms);

    uint32_t m_windowId = 0;                       //!< ID of the window to render to
    std::unique_ptr<CapEngine::TiledMap> m_map;    //!< Tiled map containing game layout and assets
    State m_state;                                 //!< Current game state
};
}  // namespace RPS

#endif /* RPS_ROCKPAPERSCISSORSSTATE_H */
