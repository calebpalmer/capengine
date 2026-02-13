#ifndef PLAYERPHYSICSCOMPONENT_H
#define PLAYERPHYSICSCOMPONENT_H

#include <capengine/components.h>
#include <capengine/gameevent.h>
#include <capengine/pcm.h>

#include <gsl/gsl-lite.hpp>

namespace Breakout {

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
};
}  // namespace Breakout

#endif  // PLAYERPHYSICSCOMPONENT_H
