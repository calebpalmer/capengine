#ifndef BALLPHYSICSCOMPONENT_H
#define BALLPHYSICSCOMPONENT_H

#include <capengine/components.h>
#include <capengine/gameevent.h>
#include <capengine/pcm.h>

#include <gsl/gsl-lite.hpp>

namespace Breakout {

class BallPhysicsComponent : public CapEngine::PhysicsComponent {
   public:
    BallPhysicsComponent(int in_ballDiameter);
    ~BallPhysicsComponent() override = default;

    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::optional<CapEngine::Rectangle> boundingPolygon(
        const CapEngine::GameObject& object) const override;
    [[nodiscard]] std::unique_ptr<CapEngine::Component> clone() const override;

   private:
    void handleGameEvent(const CapEngine::GameEvent& in_event);

    double m_ballDiameter = 0.0;
};
}  // namespace Breakout

#endif  // BALLPHYSICSCOMPONENT_H
