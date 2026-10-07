#ifndef PA_GAMEEVENTS_H
#define PA_GAMEEVENTS_H

#include <capengine/gameevent.h>

namespace PA {

struct PlayerInputEvent : public CapEngine::GameEvent {
    enum class PlayerInputType { MoveLeft, MoveRight, MoveUp, MoveDown };

    [[nodiscard]] std::string type() const override
    {
        return "PlayerInputEvent";
    };

    PlayerInputType inputType;
    bool pressed = false;
};

}  // namespace PA

#endif /* PA_GAMEEVENTS_H */
