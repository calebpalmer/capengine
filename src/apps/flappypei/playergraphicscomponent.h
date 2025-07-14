#ifndef FLAPPYPEI_PLAYERGRAPHICSCOMPONENT_H
#define FLAPPYPEI_PLAYERGRAPHICSCOMPONENT_H

#include <capengine/components.h>

namespace FlappyPei {

class PlayerGraphicsComponent final : public CapEngine::GraphicsComponent {
   public:
    void render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera, uint32_t in_windowId) override;
    void update(CapEngine::GameObject& object, double timestep) override;
    std::unique_ptr<CapEngine::Component> clone() const override;
};

}  // namespace FlappyPei

#endif  // FLAPPYPEI_PLAYERGRAPHICSCOMPONENT_H
