#ifndef PA_PLAYERPHYSICSCOMPONENT_H
#define PA_PLAYERPHYSICSCOMPONENT_H

#include <capengine/components.h>

#include <optional>

namespace PA {

class PlayerPhysicsComponent : public CapEngine::PhysicsComponent {
   public:
    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::optional<CapEngine::Rectangle> boundingPolygon(
        const CapEngine::GameObject& object) const override;
};

}  // namespace PA

#endif /* PA_PLAYERPHYSICSCOMPONENT_H */
