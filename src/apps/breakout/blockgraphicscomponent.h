#ifndef BREAKOUT_BLOCKGRAPHICSCOMPONENT_H
#define BREAKOUT_BLOCKGRAPHICSCOMPONENT_H

#include <capengine/captypes.h>
#include <capengine/colour.h>
#include <capengine/components.h>

namespace Breakout {

class BlockGraphicsComponent final : public CapEngine::GraphicsComponent {
   public:
    BlockGraphicsComponent(int in_width, int in_height, CapEngine::Colour in_colour);
    ~BlockGraphicsComponent() override = default;

    BlockGraphicsComponent(BlockGraphicsComponent const&) = default;
    BlockGraphicsComponent(BlockGraphicsComponent&&) = default;
    BlockGraphicsComponent& operator=(BlockGraphicsComponent const&) = default;
    BlockGraphicsComponent& operator=(BlockGraphicsComponent&&) = default;

    void render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera, uint32_t in_windowId) override;
    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::unique_ptr<CapEngine::Component> clone() const override;

   private:
    int m_width;
    int m_height;
    CapEngine::Colour m_colour;
};

}  // namespace Breakout

#endif  // BREAKOUT_BLOCKGRAPHICSCOMPONENT_H
