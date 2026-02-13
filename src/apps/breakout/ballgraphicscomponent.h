#ifndef BREAKOUT_BALLGRAPHICSCOMPONENT_H
#define BREAKOUT_BALLGRAPHICSCOMPONENT_H

#include <capengine/captypes.h>
#include <capengine/colour.h>
#include <capengine/components.h>

namespace Breakout {

class BallGraphicsComponent final : public CapEngine::GraphicsComponent {
   public:
    BallGraphicsComponent(int in_diameter, CapEngine::Colour in_colour);
    ~BallGraphicsComponent() override = default;

    BallGraphicsComponent(BallGraphicsComponent const&) = default;
    BallGraphicsComponent(BallGraphicsComponent&&) = default;
    BallGraphicsComponent& operator=(BallGraphicsComponent const&) = default;
    BallGraphicsComponent& operator=(BallGraphicsComponent&&) = default;

    void render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera, uint32_t in_windowId) override;
    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::unique_ptr<CapEngine::Component> clone() const override;

   private:
    int m_diameter;
    CapEngine::Colour m_colour;
};
}  // namespace Breakout

#endif  // BREAKOUT_BALLGRAPHICSCOMPONENT_H
