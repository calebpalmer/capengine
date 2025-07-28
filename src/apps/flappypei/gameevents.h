#ifndef FLAPPYPEI_GAMEEVENTS_H
#define FLAPPYPEI_GAMEEVENTS_H

#include <capengine/gameevent.h>

namespace FlappyPei {

struct PlayerInputEvent : public CapEngine::GameEvent {
    enum class PlayerInputType { Jump };
    std::string type() const override
    {
        return "PlayerInputEvent";
    };

    PlayerInputType inputType;
};

}  // namespace FlappyPei

#endif  // FLAPPYPEI_GAMEEVENTS_H
