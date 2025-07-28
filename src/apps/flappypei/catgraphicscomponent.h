#ifndef FLAPPYPEI_CATGRAPHICSCOMPONENT_H
#define FLAPPYPEI_CATGRAPHICSCOMPONENT_H

#include <capengine/captypes.h>
#include <capengine/components.h>

namespace FlappyPei {

class CatGraphicsComponent final : public CapEngine::GraphicsComponent {
   public:
    CatGraphicsComponent(int in_gapLocation, int in_gapSize);
    ~CatGraphicsComponent() override = default;

    CatGraphicsComponent(CatGraphicsComponent const&) = default;
    CatGraphicsComponent(CatGraphicsComponent&&) = default;
    CatGraphicsComponent& operator=(CatGraphicsComponent const&) = default;
    CatGraphicsComponent& operator=(CatGraphicsComponent&&) = default;

    void render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera, uint32_t in_windowId) override;
    void update(CapEngine::GameObject& object, double timestep) override;
    [[nodiscard]] std::unique_ptr<CapEngine::Component> clone() const override;

   private:
    int m_gapLocation = 0;
    int m_gapSize = 0;
    CapEngine::SharedTexturePtr m_topTexture;
    CapEngine::SharedTexturePtr m_bottomTexture;
};

}  // namespace FlappyPei

#endif  // FLAPPYPEI_CATGRAPHICSCOMPONENT_H
