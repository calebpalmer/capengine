#ifndef PA_PLAYERGRAPHICSCOMPONENT_H
#define PA_PLAYERGRAPHICSCOMPONENT_H

#include <capengine/components.h>

namespace PA {

class PlayerGraphicsComponent final : public CapEngine::GraphicsComponent {
   public:
    PlayerGraphicsComponent() = default;
    ~PlayerGraphicsComponent() override = default;

    PlayerGraphicsComponent(PlayerGraphicsComponent const&) = default;
    PlayerGraphicsComponent(PlayerGraphicsComponent&&) = default;
    PlayerGraphicsComponent& operator=(PlayerGraphicsComponent const&) = default;
    PlayerGraphicsComponent& operator=(PlayerGraphicsComponent&&) = default;

    void render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera, uint32_t in_windowId) override;
    void update(CapEngine::GameObject& object, double timestep) override;
};

}  // namespace PA

#endif /* PA_PLAYERGRAPHICSCOMPONENT_H */
