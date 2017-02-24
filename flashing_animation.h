#ifndef FLASHING_ANIMATION_H
#define FLASHING_ANIMATION_H

#include "animation.h"

#include <SDL2/SDL.h>

namespace CapEngine {

  class FlashingAnimation : public Animation {
  public:
    FlashingAnimation(Uint32 windowID, int assetID, int width, int height, int x, int y,
		      int repeatCount, double intervalMS);
    void update(double deltaTimeMS) override;
    void render() override;
    bool isDone() override;

  private:
    Uint32 m_windowID;
    int m_assetID;
    int m_width;
    int m_height;
    int m_x;
    int m_y;
    double m_intervalMS;
    double m_elapsedTimeMS;
    double m_totalTimeMS;
    bool done;
  };

}

#endif // FLASHING_ANIMATION_H
