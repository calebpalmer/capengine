#ifndef PA_PLAYERINPUTCOMPONENT_H
#define PA_PLAYERINPUTCOMPONENT_H

#include <apps/postapoc/playerphysicscomponent.h>
#include <capengine/gameevent.h>
#include <capengine/components.h>
#include <boost/signals2/connection.hpp>

namespace PA {

// forward declaration
class PlayerPhysicsComponent;

class PlayerInputComponent final : public CapEngine::InputComponent {
   public:
    PlayerInputComponent();
    void handle(const CapEngine::GameEvent& in_event);

    void update(CapEngine::GameObject& object, double timestep) override;

   private:
    friend class PlayerPhysicsComponent;

    boost::signals2::scoped_connection m_gameEventConnection;
    bool m_walkLeft = false;
    bool m_walkRight = false;
    bool m_walkUp = false;
    bool m_walkDown = false;
};

}  // namespace PA

#endif /* PA_PLAYERINPUTCOMPONENT_H */
