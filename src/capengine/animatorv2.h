#ifndef CAPENGINE_ANIMATORV2_H
#define CAPENGINE_ANIMATORV2_H

#include "asset_manager.h"

namespace CapEngine {
class AnimatorV2 {
public:
  AnimatorV2(AnimatedImage in_animatedImage);
  void update(double in_ms);
  void render(uint32_t in_windowId, Rect destRect, std::optional<double> rotationDegrees = std::nullopt);
  void start();
  bool finished() const;
  bool started() const;

private:
  int m_elapsedTime = 0;
  int m_msPerFrame = 0;
  AnimatedImage m_animatedImage;
  bool m_started = false;
  bool m_finished = false;
};
} // namespace CapEngine

#endif /* CAPENGINE_ANIMATORV2_H */
