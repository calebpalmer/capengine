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

enum class GamePhase { Start, Stopped, Choice, ChoiceEnd, Exit };
enum class Choice { Rock = 0, Paper, Scissors };

struct PlayerKeybindings {
    SDL_Keycode rockKeycode;
    SDL_Keycode paperKeycode;
    SDL_Keycode scissorsKeycode;
};
struct GlobalKeybindings {
    SDL_Keycode startKeycode;
    SDL_Keycode exitKeycode;
};

struct Settings {
    int numPlayers = 1;
    GlobalKeybindings globalKeybindings;
    PlayerKeybindings player1Keybindings;
    PlayerKeybindings player2Keybindings;
};

struct PlayerState {
    CapEngine::Rect tilePosition;
    CapEngine::Rect drawPosition;
    Choice currentChoice = Choice::Rock;
    std::optional<Choice> nextChoice = std::nullopt;
    int score = 0;
};

struct State {
    State();
    Settings settings;
    GamePhase phase = GamePhase::Start;
    PlayerState player1;
    PlayerState player2;
    int choiceTimeMs = 0;
    int currentWinner = 0;
};

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

    uint32_t m_windowId = 0;
    std::unique_ptr<CapEngine::TiledMap> m_map;
    State m_state;
};
}  // namespace RPS

#endif /* RPS_ROCKPAPERSCISSORSSTATE_H */
