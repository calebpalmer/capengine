#include "flashing_animation.h"

#include "asset_manager.h"
#include "locator.h"
#include "logger.h"

using namespace CapEngine;

FlashingAnimation::FlashingAnimation(Uint32 windowID, int assetID, int width,
                                     int height, int x, int y, int repeatCount,
                                     double intervalMS)
    : m_windowID(windowID), m_assetID(assetID), m_width(width),
      m_height(height), m_x(x), m_y(y), m_intervalMS(intervalMS),
      m_elapsedTimeMS(0.0), done(false)
{
  m_totalTimeMS = repeatCount * m_intervalMS;
}

void FlashingAnimation::update(double deltaTimeMS)
{
  m_elapsedTimeMS += deltaTimeMS;

  if (m_elapsedTimeMS > m_totalTimeMS) {
    done = true;
  }
}

void FlashingAnimation::render()
{
  if (!done) {
    int interval = m_totalTimeMS / m_elapsedTimeMS;

    // render only every other interval
    if (interval % 2 == 0) {
      // render the image
      Rectangle dstRect(m_x, m_y, m_width, m_height);
      Locator::assetManager->draw(m_windowID, m_assetID, dstRect);
    } else {
      // do not render anything
    }
  }
}

bool FlashingAnimation::isDone() { return done; }
