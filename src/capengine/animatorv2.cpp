#include "animatorv2.h"

#include "asset_manager.h"
#include "locator.h"

namespace CapEngine {

AnimatorV2::AnimatorV2(AnimatedImage in_animatedImage) : m_animatedImage(std::move(in_animatedImage)) {
  m_msPerFrame = m_animatedImage.animationTimeMs / m_animatedImage.numFrames;
}

void AnimatorV2::update(double in_ms) {
  if (m_started) {
    if (m_elapsedTime >= m_animatedImage.animationTimeMs) {
      m_finished = true;
    } else {
      m_elapsedTime += in_ms;
      if (m_elapsedTime >= m_animatedImage.animationTimeMs) {
        m_elapsedTime = m_animatedImage.animationTimeMs;
      }
    }
  }
}

void AnimatorV2::render(uint32_t in_windowId, Rect destRect, std::optional<double> rotationDegrees) {
  if (m_started && !m_finished) {
    // get the frame number: animation_time / elapsed time % num frames
    const int frameNumber = m_elapsedTime / m_msPerFrame;

    // calculate the render box based on frame number and vertical texture size / number of frames
    assert(Locator::videoManager != nullptr);
    assert(m_animatedImage.texture != nullptr);
    const auto width = Locator::videoManager->getTextureWidth(m_animatedImage.texture.get());
    const auto height = Locator::videoManager->getTextureHeight(m_animatedImage.texture.get());
    const int frameHeight = static_cast<int>(height) / m_animatedImage.numFrames;
    const int frameWidth = width; // frame takes entire width of texture.

    Rect drawRect{0, frameNumber * frameHeight, frameWidth, frameHeight};
    if (destRect.w <= 0)
      destRect.w = frameWidth;
    if (destRect.h <= 0)
      destRect.h = frameHeight;

    Locator::videoManager->drawTexture(in_windowId, m_animatedImage.texture.get(), &drawRect, &destRect,
                                       rotationDegrees);
  }
}

void AnimatorV2::start() { m_started = true; };
bool AnimatorV2::finished() const { return m_finished; }
bool AnimatorV2::started() const { return m_started; }

} // namespace CapEngine
