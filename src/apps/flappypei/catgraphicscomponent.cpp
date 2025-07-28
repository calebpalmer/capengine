#include "catgraphicscomponent.h"

#include <capengine/VideoManager.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>

#include <memory>

#include "constants.h"

namespace FlappyPei {

CatGraphicsComponent::CatGraphicsComponent(int in_gapLocation, int in_gapSize)
    : m_gapLocation(in_gapLocation), m_gapSize(in_gapSize)
{
    CapEngine::VideoManager& videoManager = CapEngine::Locator::getVideoManager();
    const CapEngine::Colour colour{235, 174, 52};
    const int height = m_gapLocation;

    if (height > 0) {
        m_topTexture = videoManager.createTexturePtr(kCatWidth, height, colour);
    }

    int bottomTextureHeight = kLogicalWindowHeight - (m_gapLocation + m_gapSize);
    if (bottomTextureHeight > 0) {
        m_bottomTexture = videoManager.createTexturePtr(kCatWidth, bottomTextureHeight, colour);
    }
}

void CatGraphicsComponent::render(CapEngine::GameObject& in_object, const CapEngine::Camera2d& in_camera,
                                  uint32_t in_windowId)
{
    CapEngine::VideoManager& videoManager = CapEngine::Locator::getVideoManager();
    CapEngine::Rect dstRect{
        .x = static_cast<int>(in_object.getPosition().getX()), .y = 0, .w = kCatWidth, .h = m_gapLocation};

    // render top rect
    if (m_topTexture != nullptr) {
        videoManager.drawTexture(in_windowId, dstRect, m_topTexture.get());
    }

    // render bottom rect
    if (m_bottomTexture != nullptr) {
        dstRect.y = m_gapLocation + m_gapSize;
        dstRect.h = kLogicalWindowHeight - (m_gapLocation + m_gapSize);
        assert(m_bottomTexture != nullptr);
        videoManager.drawTexture(in_windowId, dstRect, m_bottomTexture.get());
    }
}

void CatGraphicsComponent::update(CapEngine::GameObject& object, double timestep)
{
}

std::unique_ptr<CapEngine::Component> CatGraphicsComponent::clone() const
{
    return std::make_unique<CatGraphicsComponent>(*this);
}

}  // namespace FlappyPei
