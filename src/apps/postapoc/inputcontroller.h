#ifndef PA_INPUTCONTROLLER_H
#define PA_INPUTCONTROLLER_H

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <variant>

namespace PA {

using InputEvent = std::variant<SDL_KeyboardEvent, SDL_MouseButtonEvent, SDL_ControllerButtonEvent>;

enum class ControllerState { Main, Pause, Inventory, Dialog };

class InputController {
   public:
    void handle(InputEvent in_event);

    void enabled(bool in_enabled);

   private:
    void handle(const SDL_KeyboardEvent& in_event);
    void handle(const SDL_MouseButtonEvent& in_event);
    void handle(const SDL_ControllerButtonEvent& in_event);

    bool m_enabled = false;
    ControllerState m_state = ControllerState::Main;
};

}  // namespace PA

#endif /* PA_INPUTCONTROLLER_H */
