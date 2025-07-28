#ifndef PLAYERPHYSICSCOMPONENT_H
#define PLAYERPHYSICSCOMPONENT_H

#include <capengine/components.h>
#include <capengine/gameevent.h>

namespace FlappyPei {

class PlayerPhysicsComponent : public CapEngine::PhysicsComponent {
   public:
    PlayerPhysicsComponent();
    ~PlayerPhysicsComponent() override = default;

    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::optional<CapEngine::Rectangle> boundingPolygon(
        const CapEngine::GameObject& object) const override;
    [[nodiscard]] std::unique_ptr<CapEngine::Component> clone() const override;

   private:
    void handleGameEvent(const CapEngine::GameEvent& in_event);

    bool m_jump = false;
};
}  // namespace FlappyPei

#endif  // PLAYERPHYSICSCOMPONENT_H
