#ifndef PLAYERPHYSICSCOMPONENT_H
#define PLAYERPHYSICSCOMPONENT_H

#include <capengine/components.h>
#include <capengine/gameevent.h>
#include <capengine/pcm.h>

#include <gsl/gsl-lite.hpp>

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

    gsl::not_null<std::shared_ptr<CapEngine::PCM>> m_jumpSound;
    bool m_jump = false;
};
}  // namespace FlappyPei

#endif  // PLAYERPHYSICSCOMPONENT_H
